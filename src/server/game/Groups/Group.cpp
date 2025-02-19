/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "Group.h"
#include "Formulas.h"
#include "ObjectAccessor.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "MapManager.h"
#include "InstanceSaveMgr.h"
#include "MapInstanced.h"
#include "Util.h"
#include "LFGMgr.h"

Roll::Roll(uint64 _guid, LootItem const& li) : itemGUID(_guid), itemid(li.itemid),
itemRandomPropId(li.randomPropertyId), itemRandomSuffix(li.randomSuffix), itemCount(li.count),
totalPlayersRolling(0), totalNeed(0), totalGreed(0), totalPass(0), itemSlot(0),
rollVoteMask(ROLL_ALL_TYPE_NO_DISENCHANT)
{
}
Roll::~Roll()
{
}

void Roll::setLoot(Loot* pLoot)
{
    link(pLoot, this);
}

Loot* Roll::getLoot()
{
    return getTarget();
}

Group::Group() : m_leaderGuid(0), m_leaderName(""), m_groupType(GROUPTYPE_NORMAL),
m_dungeonDifficulty(DUNGEON_DIFFICULTY_NORMAL), m_raidDifficulty(RAID_DIFFICULTY_10MAN_NORMAL),
m_bgGroup(NULL), m_lootMethod(FREE_FOR_ALL), m_lootThreshold(ITEM_QUALITY_UNCOMMON), m_looterGuid(0),
m_subGroupsCounts(NULL), m_guid(0), m_counter(0), m_maxEnchantingLevel(0), m_dbStoreId(0)
{
    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i] = 0;
}

Group::~Group()
{
    if (m_bgGroup)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Group::~Group: battleground group being deleted.");
        if (m_bgGroup->GetBgRaid(ALLIANCE) == this) m_bgGroup->SetBgRaid(ALLIANCE, NULL);
        else if (m_bgGroup->GetBgRaid(HORDE) == this) m_bgGroup->SetBgRaid(HORDE, NULL);
        else sLog->outError("Group::~Group: battleground group is not linked to the correct battleground.");
    }
    Rolls::iterator itr;
    while (!RollId.empty())
    {
        itr = RollId.begin();
        Roll *r = *itr;
        RollId.erase(itr);
        delete(r);
    }

    // it is undefined whether objectmgr (which stores the groups) or instancesavemgr
    // will be unloaded first so we must be prepared for both cases
    // this may unload some instance saves
    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        for (BoundInstancesMap::iterator itr2 = m_boundInstances[i].begin(); itr2 != m_boundInstances[i].end(); ++itr2)
            itr2->second.save->RemoveGroup(this);

    // Sub group counters clean up
    delete[] m_subGroupsCounts;
}

bool Group::Create(Player* leader)
{
    uint64 leaderGuid = leader->GetGUID();
    uint32 lowguid = sGroupMgr->GenerateGroupId();

    m_guid = MAKE_NEW_GUID(lowguid, 0, HIGHGUID_GROUP);
    m_leaderGuid = leaderGuid;
    m_leaderName = leader->GetName();

    m_groupType  = isBGGroup() ? GROUPTYPE_BGRAID : GROUPTYPE_NORMAL;

    if (m_groupType & GROUPTYPE_RAID)
        _initRaidSubGroupsCounter();

    m_lootMethod = GROUP_LOOT;
    m_lootThreshold = ITEM_QUALITY_UNCOMMON;
    m_looterGuid = leaderGuid;

    m_dungeonDifficulty = DUNGEON_DIFFICULTY_NORMAL;
    m_raidDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;

    if (!isBGGroup())
    {
        m_dungeonDifficulty = leader->GetDungeonDifficulty();
        m_raidDifficulty = leader->GetRaidDifficulty();

        m_dbStoreId = sGroupMgr->GenerateNewGroupDbStoreId();

        sGroupMgr->RegisterGroupDbStoreId(m_dbStoreId, this);

        // store group in database
        CharacterDatabase.PExecute("INSERT INTO groups (guid, leaderGuid, lootMethod, looterGuid, lootThreshold, icon1, icon2, icon3, icon4, icon5, icon6, icon7, icon8, groupType, difficulty, raiddifficulty) "
            "VALUES ('%u', '%u', '%u', '%u', '%u', '" UI64FMTD "', '" UI64FMTD "', '" UI64FMTD "', '" UI64FMTD "', '" UI64FMTD "', '" UI64FMTD "', '" UI64FMTD "', '" UI64FMTD "', '%u', '%u', '%u')",
            m_dbStoreId, GUID_LOPART(m_leaderGuid), uint32(m_lootMethod),
            GUID_LOPART(m_looterGuid), uint32(m_lootThreshold), m_targetIcons[0], m_targetIcons[1], m_targetIcons[2], m_targetIcons[3], m_targetIcons[4], m_targetIcons[5], m_targetIcons[6],
            m_targetIcons[7], uint8(m_groupType), uint32(m_dungeonDifficulty), m_raidDifficulty);

        ASSERT(AddMember(leader)); // If the leader can't be added to a new group because it appears full, something is clearly wrong.

        Player::ConvertInstancesToGroup(leader, this, false);
    }
    else if (!AddMember(leader))
        return false;

    return true;
}

void Group::LoadGroupFromDB(Field* fields)
{
    m_dbStoreId = fields[15].GetUInt32();
    m_guid = MAKE_NEW_GUID(sGroupMgr->GenerateGroupId(), 0, HIGHGUID_GROUP);
    m_leaderGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);

    // group leader not exist
    if (!sObjectMgr->GetPlayerNameByGUID(fields[0].GetUInt32(), m_leaderName))
        return;

    m_lootMethod = LootMethod(fields[1].GetUInt8());
    m_looterGuid = MAKE_NEW_GUID(fields[2].GetUInt32(), 0, HIGHGUID_PLAYER);
    m_lootThreshold = ItemQualities(fields[3].GetUInt16());

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i] = fields[4+i].GetUInt32();

    m_groupType  = GroupType(fields[12].GetUInt8());
    if (m_groupType & GROUPTYPE_RAID)
        _initRaidSubGroupsCounter();

    uint32 diff = fields[13].GetUInt8();
    if (diff >= MAX_DUNGEON_DIFFICULTY)
        m_dungeonDifficulty = DUNGEON_DIFFICULTY_NORMAL;
    else
        m_dungeonDifficulty = Difficulty(diff);

    uint32 r_diff = fields[14].GetUInt8();
    if (r_diff >= MAX_RAID_DIFFICULTY)
       m_raidDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;
    else
       m_raidDifficulty = Difficulty(r_diff);
}

void Group::LoadMemberFromDB(uint32 guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles)
{
    MemberSlot member;
    member.guid = MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER);

    // skip non-existed member
    if (!sObjectMgr->GetPlayerNameByGUID(member.guid, member.name))
    {
        CharacterDatabase.PQuery("DELETE FROM group_member WHERE memberGuid=%u", guidLow);
        return;
    }

    member.group = subgroup;
    member.flags = memberFlags;
    member.roles = roles;

    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subgroup);
}

void Group::ConvertToLFG()
{
    m_groupType = GroupType(m_groupType | GROUPTYPE_LFG | GROUPTYPE_UNK1);
    m_lootMethod = NEED_BEFORE_GREED;
    if (!isBGGroup())
        CharacterDatabase.PExecute("UPDATE groups SET groupType='%u' WHERE guid='%u'", uint8(m_groupType), m_dbStoreId);
    SendUpdate();
}

void Group::ConvertToRaid()
{
    m_groupType = GroupType(m_groupType | GROUPTYPE_RAID);

    _initRaidSubGroupsCounter();

    if (!isBGGroup())
        CharacterDatabase.PExecute("UPDATE groups SET groupType='%u' WHERE guid='%u'", uint8(m_groupType), m_dbStoreId);
    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

bool Group::AddInvite(Player* player)
{
    if (!player || player->GetGroupInvite())
        return false;
    Group* group = player->GetGroup();
    if (group && group->isBGGroup())
        group = player->GetOriginalGroup();
    if (group)
        return false;

    RemoveInvite(player);

    m_invitees.insert(player);

    player->SetGroupInvite(this);

    sScriptMgr->OnGroupInviteMember(this, player->GetGUID());

    return true;
}

bool Group::AddLeaderInvite(Player* player)
{
    if (!AddInvite(player))
        return false;

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    return true;
}

void Group::RemoveInvite(Player* player)
{
    if (player)
    {
        m_invitees.erase(player);
        player->SetGroupInvite(NULL);
    }
}

void Group::RemoveAllInvites()
{
    for (InvitesList::iterator itr=m_invitees.begin(); itr != m_invitees.end(); ++itr)
        if (*itr)
            (*itr)->SetGroupInvite(NULL);

    m_invitees.clear();
}

Player* Group::GetInvited(uint64 guid) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetGUID() == guid)
            return (*itr);
    }
    return NULL;
}

Player* Group::GetInvited(const std::string& name) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetName() == name)
            return (*itr);
    }
    return NULL;
}

bool Group::AddMember(Player* player)
{
    // Get first not-full group
    uint8 subGroup = 0;
    if (m_subGroupsCounts)
    {
        bool groupFound = false;
        for (; subGroup < MAX_RAID_SUBGROUPS; ++subGroup)
        {
            if (m_subGroupsCounts[subGroup] < MAXGROUPSIZE)
            {
                groupFound = true;
                break;
            }
        }
        // We are raid group and no one slot is free
        if (!groupFound)
            return false;
    }

    MemberSlot member;
    member.guid      = player->GetGUID();
    member.name      = player->GetName();
    member.group     = subGroup;
    member.flags     = 0;
    member.roles     = 0;
    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subGroup);

    if (player)
    {
        player->SetGroupInvite(NULL);
        if (player->GetGroup() && isBGGroup()) //if player is in group and he is being added to BG raid group, then call SetBattlegroundRaid()
            player->SetBattlegroundRaid(this, subGroup);
        else if (player->GetGroup()) //if player is in bg raid and we are adding him to normal group, then call SetOriginalGroup()
            player->SetOriginalGroup(this, subGroup);
        else //if player is not in group, then call set group
            player->SetGroup(this, subGroup);

        // if the same group invites the player back, cancel the homebind timer
        InstanceGroupBind* bind = GetBoundInstance(player);
        if (bind && bind->save->GetInstanceId() == player->GetInstanceId())
            player->m_InstanceValid = true;
    }

    if (!isRaidGroup())                                      // reset targetIcons for non-raid-groups
    {
        for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
            m_targetIcons[i] = 0;
    }

    // insert into the table if we're not a battleground group
    if (!isBGGroup())
        CharacterDatabase.PExecute("INSERT INTO group_member (guid, memberGuid, memberFlags, subgroup, roles) VALUES(%u, %u, %u, %u, %u)",
                                    m_dbStoreId, GUID_LOPART(member.guid), member.flags, member.group, member.roles);

    SendUpdate();
    sScriptMgr->OnGroupAddMember(this, player->GetGUID());

    if (player)
    {
        if (!IsLeader(player->GetGUID()) && !isBGGroup())
        {
            // reset the new member's instances, unless he is currently in one of them
            // including raid/heroic instances that they are not permanently bound to!
            player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, false);
            player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true);

            if (player->getLevel() >= LEVELREQUIREMENT_HEROIC)
            {
                if (player->GetDungeonDifficulty() != GetDungeonDifficulty())
                {
                    player->SetDungeonDifficulty(GetDungeonDifficulty());
                    player->SendDungeonDifficulty(true);
                }
                if (player->GetRaidDifficulty() != GetRaidDifficulty())
                {
                    player->SetRaidDifficulty(GetRaidDifficulty());
                    player->SendRaidDifficulty(true);
                }
            }
        }
        player->SetGroupUpdateFlag(GROUP_UPDATE_FULL);
        UpdatePlayerOutOfRange(player);

        // quest related GO state dependent from raid membership
        if (isRaidGroup())
            player->UpdateForQuestWorldObjects();

        if (m_maxEnchantingLevel < player->GetSkillValue(SKILL_ENCHANTING))
            m_maxEnchantingLevel = player->GetSkillValue(SKILL_ENCHANTING);
    }

    return true;
}

bool Group::RemoveMember(uint64 guid, const RemoveMethod &method /*= GROUP_REMOVEMETHOD_DEFAULT*/, uint64 kicker /*= 0*/, const char* reason /*= NULL*/)
{
    BroadcastGroupUpdate();

    sScriptMgr->OnGroupRemoveMember(this, guid, method, kicker, reason);

    // LFG group vote kick handled in scripts
    if (isLFGGroup() && method == GROUP_REMOVEMETHOD_KICK)
        return m_memberSlots.size();

    // remove member and change leader (if need) only if strong more 2 members _before_ member remove (BG allow 1 member group)
    if (GetMembersCount() > (isBGGroup() ? 1u : 2u))
    {
        Player* player = ObjectAccessor::FindPlayer(guid);
        if (player)
        {
            // Battleground group handling
            if (isBGGroup())
                player->RemoveFromBattlegroundRaid();
            else
            // Regular group
            {
                if (player->GetOriginalGroup() == this)
                    player->SetOriginalGroup(NULL);
                else
                    player->SetGroup(NULL);

                // quest related GO state dependent from raid membership
                player->UpdateForQuestWorldObjects();
            }

            WorldPacket data;

            if (method == GROUP_REMOVEMETHOD_KICK)
            {
                data.Initialize(SMSG_GROUP_UNINVITE, 0);
                player->GetSession()->SendPacket(&data);
            }

            // Do we really need to send this opcode?
            data.Initialize(SMSG_GROUP_LIST, 1+1+1+1+8+4+4+8);
            data << uint8(0x10) << uint8(0) << uint8(0) << uint8(0);
            data << uint64(m_guid) << uint32(m_counter) << uint32(0) << uint64(0);
            player->GetSession()->SendPacket(&data);

            _homebindIfInstance(player);
        }

        // Remove player from group in DB
        CharacterDatabase.PExecute("DELETE FROM group_member WHERE memberGuid=%u", GUID_LOPART(guid));

        // Reevaluate group enchanter if the leaving player had enchanting skill or the player is offline
        if ((player && player->GetSkillValue(SKILL_ENCHANTING)) || !player)
            ResetMaxEnchantingLevel();

        // Remove player from loot rolls
        for (Rolls::iterator it = RollId.begin(); it != RollId.end(); ++it)
        {
            Roll* roll = *it;
            Roll::PlayerVote::iterator itr2 = roll->playerVote.find(guid);
            if (itr2 == roll->playerVote.end())
                continue;

            if (itr2->second == GREED || itr2->second == DISENCHANT)
                --roll->totalGreed;
            else if (itr2->second == NEED)
                --roll->totalNeed;
            else if (itr2->second == PASS)
                --roll->totalPass;

            if (itr2->second != NOT_VALID)
                --roll->totalPlayersRolling;

            roll->playerVote.erase(itr2);

            CountRollVote(guid, roll->itemGUID, GetMembersCount()-1, MAX_ROLL_TYPE);
        }

        // Update subgroups
        member_witerator slot = _getMemberWSlot(guid);
        if (slot != m_memberSlots.end())
        {
            SubGroupCounterDecrease(slot->group);
            m_memberSlots.erase(slot);
        }

        // Pick new leader if necessary
        if (m_leaderGuid == guid)
        {
            for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
            {
                if (ObjectAccessor::FindPlayer(itr->guid))
                {
                    ChangeLeader(itr->guid);
                    break;
                }
            }
        }

        SendUpdate();

        return true;
    }
    // If group size before player removal <= 2 then disband it
    else
    {
        Disband();
        return false;
    }
}

void Group::ChangeLeader(uint64 guid)
{
    member_witerator slot = _getMemberWSlot(guid);

    if (slot == m_memberSlots.end())
        return;

    Player* player = ObjectAccessor::FindPlayer(slot->guid);

    // Don't allow switching leader to offline players
    if (!player)
        return;

    sScriptMgr->OnGroupChangeLeader(this, m_leaderGuid, guid);

    if (!isBGGroup())
    {
        // Remove the groups permanent instance bindings
        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        {
            for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin(); itr != m_boundInstances[i].end();)
            {
                if (itr->second.perm)
                {
                    itr->second.save->RemoveGroup(this);
                    m_boundInstances[i].erase(itr++);
                }
                else
                    ++itr;
            }
        }

        // Same in the database
        CharacterDatabase.PExecute("DELETE FROM group_instance WHERE guid=%u AND (permanent = 1 OR instance IN (SELECT instance FROM character_instance WHERE guid = '%u'))",
            m_dbStoreId, player->GetGUIDLow());

        // Copy the permanent binds from the new leader to the group
        Player::ConvertInstancesToGroup(player, this, true);

        // update the group leader
        CharacterDatabase.PExecute("UPDATE groups SET leaderGuid='%u' WHERE guid='%u'", player->GetGUIDLow(), m_dbStoreId);
    }

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    ToggleGroupMemberFlag(slot, MEMBER_FLAG_ASSISTANT, false);

    WorldPacket data(SMSG_GROUP_SET_LEADER, m_leaderName.size()+1);
    data << slot->name;
    BroadcastPacket(&data, true);
}

void Group::Disband(bool hideDestroy /* = false */)
{
    sScriptMgr->OnGroupDisband(this);

    Player* player;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        player = ObjectAccessor::FindPlayer(citr->guid);
        if (!player)
            continue;

        //we cannot call _removeMember because it would invalidate member iterator
        //if we are removing player from battleground raid
        if (isBGGroup())
            player->RemoveFromBattlegroundRaid();
        else
        {
            //we can remove player who is in battleground from his original group
            if (player->GetOriginalGroup() == this)
                player->SetOriginalGroup(NULL);
            else
                player->SetGroup(NULL);
        }

        // quest related GO state dependent from raid membership
        if (isRaidGroup())
            player->UpdateForQuestWorldObjects();

        if (!player->GetSession())
            continue;

        WorldPacket data;
        if (!hideDestroy)
        {
            data.Initialize(SMSG_GROUP_DESTROYED, 0);
            player->GetSession()->SendPacket(&data);
        }

        //we already removed player from group and in player->GetGroup() is his original group, send update
        if (Group* group = player->GetGroup())
        {
            group->SendUpdate();
        }
        else
        {
            data.Initialize(SMSG_GROUP_LIST, 1+1+1+1+8+4+4+8);
            data << uint8(0x10) << uint8(0) << uint8(0) << uint8(0);
            data << uint64(m_guid) << uint32(m_counter) << uint32(0) << uint64(0);
            player->GetSession()->SendPacket(&data);
        }

        _homebindIfInstance(player);
    }
    RollId.clear();
    m_memberSlots.clear();

    RemoveAllInvites();

    if (!isBGGroup())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        trans->PAppend("DELETE FROM groups WHERE guid = %u", m_dbStoreId);
        trans->PAppend("DELETE FROM group_member WHERE guid = %u", m_dbStoreId);
        CharacterDatabase.CommitTransaction(trans);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, false, NULL);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, NULL);

        sGroupMgr->FreeGroupDbStoreId(this);
    }

    sGroupMgr->RemoveGroup(this);
    delete this;
}

/*********************************************************/
/***                   LOOT SYSTEM                     ***/
/*********************************************************/

void Group::SendLootStartRoll(uint32 CountDown, uint32 mapid, const Roll &r)
{
    WorldPacket data(SMSG_LOOT_START_ROLL, (8+4+4+4+4+4+4+1));
    data << uint64(r.itemGUID);                             // guid of rolled item
    data << uint32(mapid);                                  // 3.3.3 mapid
    data << uint32(r.totalPlayersRolling);                  // maybe the number of players rolling for it???
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId);                     // item random property ID
    data << uint32(r.itemCount);                            // items in stack
    data << uint32(CountDown);                              // the countdown time to choose "need" or "greed"
    data << uint8(r.rollVoteMask);                          // roll type mask

    for (Roll::PlayerVote::const_iterator itr=r.playerVote.begin(); itr != r.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second == NOT_EMITED_YET)
            p->GetSession()->SendPacket(&data);
    }
}

void Group::SendLootRoll(uint64 SourceGuid, uint64 TargetGuid, uint8 RollNumber, uint8 RollType, const Roll &r)
{
    WorldPacket data(SMSG_LOOT_ROLL, (8+4+8+4+4+4+1+1+1));
    data << uint64(SourceGuid);                             // guid of the item rolled
    data << uint32(0);                                      // unknown, maybe amount of players
    data << uint64(TargetGuid);
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId);                     // Item random property ID
    data << uint8(RollNumber);                              // 0: "Need for: [item name]" > 127: "you passed on: [item name]"      Roll number
    data << uint8(RollType);                                // 0: "Need for: [item name]" 0: "You have selected need for [item name] 1: need roll 2: greed roll
    data << uint8(0);                                       // auto pass on NeedBeforeGreed loot because player cannot use the object

    for (Roll::PlayerVote::const_iterator itr=r.playerVote.begin(); itr != r.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            p->GetSession()->SendPacket(&data);
    }
}

void Group::SendLootRollWon(uint64 SourceGuid, uint64 TargetGuid, uint8 RollNumber, uint8 RollType, const Roll &r)
{
    WorldPacket data(SMSG_LOOT_ROLL_WON, (8+4+4+4+4+8+1+1));
    data << uint64(SourceGuid);                             // guid of the item rolled
    data << uint32(0);                                      // unknown, maybe amount of players
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId);                     // Item random property
    data << uint64(TargetGuid);                             // guid of the player who won.
    data << uint8(RollNumber);                              // rollnumber realted to SMSG_LOOT_ROLL
    data << uint8(RollType);                                // Rolltype related to SMSG_LOOT_ROLL

    for (Roll::PlayerVote::const_iterator itr=r.playerVote.begin(); itr != r.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            p->GetSession()->SendPacket(&data);
    }
}

void Group::SendLootAllPassed(uint32 NumberOfPlayers, const Roll &r)
{
    WorldPacket data(SMSG_LOOT_ALL_PASSED, (8+4+4+4+4));
    data << uint64(r.itemGUID);                             // Guid of the item rolled
    data << uint32(NumberOfPlayers);                        // The number of players rolling for it???
    data << uint32(r.itemid);                               // The itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomPropId);                     // Item random property ID
    data << uint32(r.itemRandomSuffix);                     // Item random suffix ID

    for (Roll::PlayerVote::const_iterator itr=r.playerVote.begin(); itr != r.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            p->GetSession()->SendPacket(&data);
    }
}

// notify group members which player is the allowed looter for the given creature
void Group::SendLooter(Creature* creature, Player* pLooter)
{
    ASSERT(creature);

    WorldPacket data(SMSG_LOOT_LIST, (8+8));
    data << uint64(creature->GetGUID());
    data << uint8(0); // unk1

    if (pLooter)
        data.append(pLooter->GetPackGUID());
    else
        data << uint8(0);

    BroadcastPacket(&data, false);
}

void Group::GroupLoot(Loot* loot, WorldObject* pLootedObject)
{
    std::vector<LootItem>::iterator i;
    ItemTemplate const* item;
    uint8 itemSlot = 0;

    for (i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //sLog->outDebug("Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
            Roll* r = new Roll(newitemGUID, *i);

            //a vector is filled with only near party members
            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->getSource();
                if (!member || !member->GetSession())
                    continue;
                if (i->AllowedForPlayer(member))
                {
                    if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                    {
                        r->totalPlayersRolling++;

                        if (member->GetPassOnGroupLoot())
                        {
                            r->playerVote[member->GetGUID()] = PASS;
                            r->totalPass++;
                            // can't broadcast the pass now. need to wait until all rolling players are known.
                        }
                        else
                            r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
                    }
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                loot->items[itemSlot].is_blocked = true;

                // If there is any "auto pass", broadcast the pass now.
                if (r->totalPass)
                {
                    for (Roll::PlayerVote::const_iterator itr=r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                    {
                        Player* p = ObjectAccessor::FindPlayer(itr->first);
                        if (!p || !p->GetSession())
                            continue;

                        if (itr->second == PASS)
                            SendLootRoll(newitemGUID, p->GetGUID(), 128, ROLL_PASS, *r);
                    }
                }

                SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

                RollId.push_back(r);

                if (Creature* creature = pLootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = pLootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }
}

void Group::NeedBeforeGreed(Loot* loot, WorldObject* pLootedObject)
{
    ItemTemplate const* item;
    uint8 itemSlot = 0;
    for (std::vector<LootItem>::iterator i=loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
            Roll* r=new Roll(newitemGUID, *i);

            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* playerToRoll = itr->getSource();
                if (!playerToRoll || !playerToRoll->GetSession())
                    continue;

                bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
                if (playerToRoll->CanUseItem(item) == EQUIP_ERR_OK && allowedForPlayer)
                {
                    if (playerToRoll->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                    {
                        r->totalPlayersRolling++;

                        if (playerToRoll->GetPassOnGroupLoot())
                        {
                            r->playerVote[playerToRoll->GetGUID()] = PASS;
                            r->totalPass++;
                            // can't broadcast the pass now. need to wait until all rolling players are known.
                        }
                        else
                            r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
                    }
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                if (item->Flags2 & ITEM_FLAGS_EXTRA_NEED_ROLL_DISABLED)
                    r->rollVoteMask &= ~ROLL_FLAG_TYPE_NEED;

                loot->items[itemSlot].is_blocked = true;

                // If there is any "auto pass", broadcast the pass now.
                if (r->totalPass)
                {
                    for (Roll::PlayerVote::const_iterator itr=r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                    {
                        Player* p = ObjectAccessor::FindPlayer(itr->first);
                        if (!p || !p->GetSession())
                            continue;

                        if (itr->second == PASS)
                            SendLootRoll(newitemGUID, p->GetGUID(), 128, ROLL_PASS, *r);
                    }
                }

                SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

                RollId.push_back(r);

                if (Creature* creature = pLootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }
}

void Group::MasterLoot(Loot* /*loot*/, WorldObject* pLootedObject)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Group::MasterLoot (SMSG_LOOT_MASTER_LIST, 330)");

    uint32 real_count = 0;

    WorldPacket data(SMSG_LOOT_MASTER_LIST, 330);
    data << (uint8)GetMembersCount();

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* looter = itr->getSource();
        if (!looter->IsInWorld())
            continue;

        if (looter->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
        {
            data << uint64(looter->GetGUID());
            ++real_count;
        }
    }

    data.put<uint8>(0, real_count);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* looter = itr->getSource();
        if (looter->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            looter->GetSession()->SendPacket(&data);
    }
}

void Group::CountRollVote(uint64 playerGUID, uint64 Guid, uint32 NumberOfPlayers, uint8 Choice)
{
    Rolls::iterator rollI = GetRoll(Guid);
    if (rollI == RollId.end())
        return;
    Roll* roll = *rollI;

    Roll::PlayerVote::iterator itr = roll->playerVote.find(playerGUID);
    // this condition means that player joins to the party after roll begins
    if (itr == roll->playerVote.end())
        return;

    if (roll->getLoot())
        if (roll->getLoot()->items.empty())
            return;

    switch (Choice)
    {
        case ROLL_PASS:                                     // Player choose pass
            SendLootRoll(0, playerGUID, 128, ROLL_PASS, *roll);
            ++roll->totalPass;
            itr->second = PASS;
            break;
        case ROLL_NEED:                                     // player choose Need
            SendLootRoll(0, playerGUID, 0, 0, *roll);
            ++roll->totalNeed;
            itr->second = NEED;
            break;
        case ROLL_GREED:                                    // player choose Greed
            SendLootRoll(0, playerGUID, 128, ROLL_GREED, *roll);
            ++roll->totalGreed;
            itr->second = GREED;
            break;
        case ROLL_DISENCHANT:                               // player choose Disenchant
            SendLootRoll(0, playerGUID, 128, ROLL_DISENCHANT, *roll);
            ++roll->totalGreed;
            itr->second = DISENCHANT;
            break;
    }

    if (roll->totalPass + roll->totalNeed + roll->totalGreed >= roll->totalPlayersRolling)
        CountTheRoll(rollI, NumberOfPlayers);
}

//called when roll timer expires
void Group::EndRoll(Loot* pLoot)
{
    for (Rolls::iterator itr = RollId.begin(); itr != RollId.end();)
    {
        if ((*itr)->getLoot() == pLoot) {
            CountTheRoll(itr, GetMembersCount());           //i don't have to edit player votes, who didn't vote ... he will pass
            itr = RollId.begin();
        }
        else
            ++itr;
    }
}

void Group::CountTheRoll(Rolls::iterator rollI, uint32 NumberOfPlayers)
{
    Roll* roll = *rollI;
    if (!roll->isValid())                                   // is loot already deleted ?
    {
        RollId.erase(rollI);
        delete roll;
        return;
    }

    //end of the roll
    if (roll->totalNeed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            uint64 maxguid  = (*roll->playerVote.begin()).first;
            Player* player;

            for (Roll::PlayerVote::const_iterator itr=roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != NEED)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(0, itr->first, randomN, ROLL_NEED, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                }
            }
            SendLootRollWon(0, maxguid, maxresul, ROLL_NEED, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT, roll->itemid, maxresul);

                ItemPosCountVec dest;
                LootItem* item = &(roll->getLoot()->items[roll->itemSlot]);
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                if (msg == EQUIP_ERR_OK)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->unlootedCount--;
                    AllowedLooterSet looters = item->GetAllowedLooters();
                    player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, looters);
                }
                else
                {
                    item->is_blocked = false;
                    player->SendEquipError(msg, NULL, NULL, roll->itemid);
                }
            }
        }
    }
    else if (roll->totalGreed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            uint64 maxguid = (*roll->playerVote.begin()).first;
            Player* player;
            RollVote rollvote = NOT_VALID;

            Roll::PlayerVote::iterator itr;
            for (itr = roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != GREED && itr->second != DISENCHANT)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(0, itr->first, randomN, itr->second, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                    rollvote = itr->second;
                }
            }
            SendLootRollWon(0, maxguid, maxresul, rollvote, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT, roll->itemid, maxresul);

                LootItem* item = &(roll->getLoot()->items[roll->itemSlot]);

                if (rollvote == GREED)
                {
                    ItemPosCountVec dest;
                    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                    if (msg == EQUIP_ERR_OK)
                    {
                        item->is_looted = true;
                        roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                        roll->getLoot()->unlootedCount--;
                        AllowedLooterSet looters = item->GetAllowedLooters();
                        player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, looters);
                    }
                    else
                    {
                        item->is_blocked = false;
                        player->SendEquipError(msg, NULL, NULL, roll->itemid);
                    }
                }
                else if (rollvote == DISENCHANT)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->unlootedCount--;
                    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(roll->itemid);
                    player->AutoStoreLoot(pProto->DisenchantID, LootTemplates_Disenchant, true);
                }
            }
        }
    }
    else
    {
        SendLootAllPassed(NumberOfPlayers, *roll);

        // remove is_blocked so that the item is lootable by all players
        LootItem* item = &(roll->getLoot()->items[roll->itemSlot]);
        if (item)
            item->is_blocked = false;
    }

    RollId.erase(rollI);
    delete roll;
}

void Group::SetTargetIcon(uint8 id, uint64 whoGuid, uint64 targetGuid)
{
    if (id >= TARGETICONCOUNT)
        return;

    // clean other icons
    if (targetGuid != 0)
        for (int i=0; i<TARGETICONCOUNT; ++i)
            if (m_targetIcons[i] == targetGuid)
                SetTargetIcon(i, 0, 0);

    m_targetIcons[id] = targetGuid;

    WorldPacket data(MSG_RAID_TARGET_UPDATE, (1+8+1+8));
    data << uint8(0);                                       // set targets
    data << uint64(whoGuid);
    data << uint8(id);
    data << uint64(targetGuid);
    BroadcastPacket(&data, true);
}

void Group::SendTargetIconList(WorldSession* session)
{
    if (!session)
        return;

    WorldPacket data(MSG_RAID_TARGET_UPDATE, (1+TARGETICONCOUNT*9));
    data << uint8(1);                                       // list targets

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
    {
        if (m_targetIcons[i] == 0)
            continue;

        data << uint8(i);
        data << uint64(m_targetIcons[i]);
    }

    session->SendPacket(&data);
}

void Group::SendUpdate()
{
    for (member_witerator witr = m_memberSlots.begin(); witr != m_memberSlots.end(); ++witr)
    {
        SendUpdateToPlayer(witr->guid, &(*witr));
    }
}

void Group::SendUpdateToPlayer(uint64 playerGUID, MemberSlot* slot)
{
    Player* player = ObjectAccessor::FindPlayer(playerGUID);

    if (!player || !player->GetSession() || player->GetGroup() != this)
        return;

    // if MemberSlot wasn't provided
    if (!slot)
    {
        member_witerator witr = _getMemberWSlot(playerGUID);

        if (witr == m_memberSlots.end()) // if there is no MemberSlot for such a player
            return;

        slot = &(*witr);
    }

    WorldPacket data(SMSG_GROUP_LIST, (1+1+1+1+1+4+8+4+4+(GetMembersCount()-1)*(13+8+1+1+1+1)+8+1+8+1+1+1+1));
    data << uint8(m_groupType);                         // group type (flags in 3.3)
    data << uint8(slot->group);
    data << uint8(slot->flags);
    data << uint8(slot->roles);
    if (isLFGGroup())
    {
        data << uint8(sLFGMgr->GetState(m_guid) == LFG_STATE_FINISHED_DUNGEON ? 2 : 0); // FIXME - Dungeon save status? 2 = done
        data << uint32(sLFGMgr->GetDungeon(m_guid));
    }

    data << uint64(m_guid);
    data << uint32(m_counter++);                        // 3.3, value increases every time this packet gets sent
    data << uint32(GetMembersCount()-1);
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        if (slot->guid == citr->guid)
            continue;

        Player* member = ObjectAccessor::FindPlayer(citr->guid);

        uint8 onlineState = (member) ? MEMBER_STATUS_ONLINE : MEMBER_STATUS_OFFLINE;
        onlineState = onlineState | ((isBGGroup()) ? MEMBER_STATUS_PVP : 0);

        data << citr->name;
        data << uint64(citr->guid);                     // guid
        data << uint8(onlineState);                     // online-state
        data << uint8(citr->group);                     // groupid
        data << uint8(citr->flags);                     // See enum GroupMemberFlags
        data << uint8(citr->roles);                     // Lfg Roles
    }

    data << uint64(m_leaderGuid);                       // leader guid

    if (GetMembersCount() - 1)
    {
        data << uint8(m_lootMethod);                    // loot method
        data << uint64(m_looterGuid);                   // looter guid
        data << uint8(m_lootThreshold);                 // loot threshold
        data << uint8(m_dungeonDifficulty);             // Dungeon Difficulty
        data << uint8(m_raidDifficulty);                // Raid Difficulty
        data << uint8(0);                               // 3.3
    }

    player->GetSession()->SendPacket(&data);
}

void Group::UpdatePlayerOutOfRange(Player* player)
{
    if (!player || !player->IsInWorld())
        return;

    WorldPacket data;
    player->GetSession()->BuildPartyMemberStatsChangedPacket(player, &data);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        player = itr->getSource();
        if (player && !player->IsWithinDist(player, player->GetSightRange(), false))
            player->GetSession()->SendPacket(&data);
    }
}

void Group::BroadcastPacket(WorldPacket* packet, bool ignorePlayersInBGRaid, int group, uint64 ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* pl = itr->getSource();
        if (!pl || (ignore != 0 && pl->GetGUID() == ignore) || (ignorePlayersInBGRaid && pl->GetGroup() != this))
            continue;

        if (pl->GetSession() && (group == -1 || itr->getSubGroup() == group))
            pl->GetSession()->SendPacket(packet);
    }
}

void Group::BroadcastReadyCheck(WorldPacket* packet)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* pl = itr->getSource();
        if (pl && pl->GetSession())
            if (IsLeader(pl->GetGUID()) || IsAssistant(pl->GetGUID()))
                pl->GetSession()->SendPacket(packet);
    }
}

void Group::OfflineReadyCheck()
{
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* pl = ObjectAccessor::FindPlayer(citr->guid);
        if (!pl || !pl->GetSession())
        {
            WorldPacket data(MSG_RAID_READY_CHECK_CONFIRM, 9);
            data << uint64(citr->guid);
            data << uint8(0);
            BroadcastReadyCheck(&data);
        }
    }
}

bool Group::_setMembersGroup(uint64 guid, uint8 group)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return false;

    slot->group = group;

    SubGroupCounterIncrease(group);

    if (!isBGGroup())
        CharacterDatabase.PExecute("UPDATE group_member SET subgroup='%u' WHERE memberGuid='%u'", group, GUID_LOPART(guid));

    return true;
}

bool Group::SameSubGroup(Player const* member1, Player const* member2) const
{
    if (!member1 || !member2)
        return false;
    if (member1->GetGroup() != this || member2->GetGroup() != this)
        return false;
    else
        return member1->GetSubGroup() == member2->GetSubGroup();
}

// Allows setting sub groups both for online or offline members
void Group::ChangeMembersGroup(uint64 guid, uint8 group)
{
    // Only raid groups have sub groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Abort if the player is already in the target sub group
    uint8 prevSubGroup = GetMemberGroup(guid);
    if (prevSubGroup == group)
        return;

    // Update the player slot with the new sub group setting
    slot->group = group;

    // Increase the counter of the new sub group..
    SubGroupCounterIncrease(group);

    // ..and decrease the counter of the previous one
    SubGroupCounterDecrease(prevSubGroup);

    // Preserve new sub group in database for non-raid groups
    if (!isBGGroup())
        CharacterDatabase.PExecute("UPDATE group_member SET subgroup='%u' WHERE memberGuid='%u'", group, GUID_LOPART(guid));

    Player* player = ObjectAccessor::FindPlayer(guid);

    // In case the moved player is online, update the player object with the new sub group references
    if (player)
    {
        if (player->GetGroup() == this)
            player->GetGroupRef().setSubGroup(group);
        else
        {
            // If player is in BG raid, it is possible that he is also in normal raid - and that normal raid is stored in m_originalGroup reference
            prevSubGroup = player->GetOriginalSubGroup();
            player->GetOriginalGroupRef().setSubGroup(group);
        }
    }

    // Broadcast the changes to the group
    SendUpdate();
}

// Retrieve the next Round-Roubin player for the group
//
// No update done if loot method is Master or FFA.
//
// If the RR player is not yet set for the group, the first group member becomes the round-robin player.
// If the RR player is set, the next player in group becomes the round-robin player.
//
// If ifneed is true,
//      the current RR player is checked to be near the looted object.
//      if yes, no update done.
//      if not, he loses his turn.
void Group::UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed)
{
    switch (GetLootMethod())
    {
        case MASTER_LOOT:
        case FREE_FOR_ALL:
            return;
        default:
            // round robin style looting applies for all low
            // quality items in each loot method except free for all and master loot
            break;
    }

    uint64 oldLooterGUID = GetLooterGuid();
    member_citerator guid_itr = _getMemberCSlot(oldLooterGUID);
    if (guid_itr != m_memberSlots.end())
    {
        if (ifneed)
        {
            // not update if only update if need and ok
            Player* looter = ObjectAccessor::FindPlayer(guid_itr->guid);
            if (looter && looter->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                return;
        }
        ++guid_itr;
    }

    // search next after current
    Player* pNewLooter = NULL;
    for (member_citerator itr = guid_itr; itr != m_memberSlots.end(); ++itr)
    {
        if (Player* pl = ObjectAccessor::FindPlayer(itr->guid))
            if (pl->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                pNewLooter = pl;
                break;
            }
    }

    if (!pNewLooter)
    {
        // search from start
        for (member_citerator itr = m_memberSlots.begin(); itr != guid_itr; ++itr)
        {
            if (Player* pl = ObjectAccessor::FindPlayer(itr->guid))
                if (pl->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    pNewLooter = pl;
                    break;
                }
        }
    }

    if (pNewLooter)
    {
        if (oldLooterGUID != pNewLooter->GetGUID())
        {
            SetLooterGuid(pNewLooter->GetGUID());
            SendUpdate();
        }
    }
    else
    {
        SetLooterGuid(0);
        SendUpdate();
    }
}

GroupJoinBattlegroundResult Group::CanJoinBattlegroundQueue(Battleground const* bgOrTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 /*MaxPlayerCount*/, bool isRated, uint32 arenaSlot)
{
    // check if this group is LFG group
    if (isLFGGroup())
        return ERR_LFG_CANT_USE_BATTLEGROUND;

    BattlemasterListEntry const* bgEntry = sBattlemasterListStore.LookupEntry(bgOrTemplate->GetTypeID());
    if (!bgEntry)
        return ERR_GROUP_JOIN_BATTLEGROUND_FAIL;            // shouldn't happen

    // check for min / max count
    uint32 memberscount = GetMembersCount();

    if (memberscount > bgEntry->maxGroupSize)                // no MinPlayerCount for battlegrounds
        return ERR_BATTLEGROUND_NONE;                        // ERR_GROUP_JOIN_BATTLEGROUND_TOO_MANY handled on client side

    // get a player as reference, to compare other players' stats to (arena team id, queue id based on level, etc.)
    Player* reference = GetFirstMember()->getSource();
    // no reference found, can't join this way
    if (!reference)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bgOrTemplate->GetMapId(), reference->getLevel());
    if (!bracketEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    uint32 arenaTeamId = reference->GetArenaTeamId(arenaSlot);
    uint32 team = reference->GetTeam();

    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // check every member of the group to be able to join
    memberscount = 0;
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next(), ++memberscount)
    {
        Player* member = itr->getSource();
        // offline member? don't let join
        if (!member)
            return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't allow cross-faction join as group
        if (member->GetTeam() != team)
            return ERR_BATTLEGROUND_JOIN_TIMED_OUT;
        // not in the same battleground level braket, don't let join
        PvPDifficultyEntry const* memberBracketEntry = GetBattlegroundBracketByLevel(bracketEntry->mapId, member->getLevel());
        if (memberBracketEntry != bracketEntry)
            return ERR_BATTLEGROUND_JOIN_RANGE_INDEX;
        // don't let join rated matches if the arena team id doesn't match
        if (isRated && member->GetArenaTeamId(arenaSlot) != arenaTeamId)
            return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't let join if someone from the group is already in that bg queue
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeId))
            return ERR_BATTLEGROUND_JOIN_FAILED;            // not blizz-like
        // don't let join if someone from the group is in bg queue random
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeIdRandom))
            return ERR_IN_RANDOM_BG;
        // don't let join to bg queue random if someone from the group is already in bg queue
        if (bgOrTemplate->GetTypeID() == BATTLEGROUND_RB && member->InBattlegroundQueue())
            return ERR_IN_NON_RANDOM_BG;
        // check for deserter debuff in case not arena queue
        if (bgOrTemplate->GetTypeID() != BATTLEGROUND_AA && !member->CanJoinToBattleground())
            return ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS;
        // check if member can join any more battleground queues
        if (!member->HasFreeBattlegroundQueueId())
            return ERR_BATTLEGROUND_TOO_MANY_QUEUES;        // not blizz-like
        // check if someone in party is using dungeon system
        if (member->isUsingLfg())
            return ERR_LFG_CANT_USE_BATTLEGROUND;
    }

    // only check for MinPlayerCount since MinPlayerCount == MaxPlayerCount for arenas...
    if (bgOrTemplate->isArena() && memberscount != MinPlayerCount)
        return ERR_ARENA_TEAM_PARTY_SIZE;

    return GroupJoinBattlegroundResult(bgOrTemplate->GetTypeID());
}

//===================================================
//============== Roll ===============================
//===================================================

void Roll::targetObjectBuildLink()
{
    // called from link()
    getTarget()->addLootValidatorRef(this);
}

void Group::SetDungeonDifficulty(Difficulty difficulty)
{
    m_dungeonDifficulty = difficulty;
    if (!isBGGroup())
       CharacterDatabase.PExecute("UPDATE groups SET difficulty = %u WHERE guid ='%u'", m_dungeonDifficulty, m_dbStoreId);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetDungeonDifficulty(difficulty);
        player->SendDungeonDifficulty(true);
    }
}

void Group::SetRaidDifficulty(Difficulty difficulty)
{
    m_raidDifficulty = difficulty;
    if (!isBGGroup())
        CharacterDatabase.PExecute("UPDATE groups SET raiddifficulty = %u WHERE guid ='%u'", m_raidDifficulty, m_dbStoreId);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetRaidDifficulty(difficulty);
        player->SendRaidDifficulty(true);
    }
}

bool Group::InCombatToInstance(uint32 instanceId)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (player && !player->getAttackers().empty() && player->GetInstanceId() == instanceId && (player->GetMap()->IsRaidOrHeroicDungeon()))
            for (std::set<Unit*>::const_iterator i = player->getAttackers().begin(); i != player->getAttackers().end(); ++i)
                if ((*i) && (*i)->GetTypeId() == TYPEID_UNIT && (*i)->ToCreature()->GetCreatureInfo()->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
                    return true;
    }
    return false;
}

void Group::ResetInstances(uint8 method, bool isRaid, Player* SendMsgTo)
{
    if (isBGGroup())
        return;

    // method can be INSTANCE_RESET_ALL, INSTANCE_RESET_CHANGE_DIFFICULTY, INSTANCE_RESET_GROUP_DISBAND

    // we assume that when the difficulty changes, all instances that can be reset will be
    Difficulty diff = GetDifficulty(isRaid);

    for (BoundInstancesMap::iterator itr = m_boundInstances[diff].begin(); itr != m_boundInstances[diff].end();)
    {
        InstanceSave* p = itr->second.save;
        const MapEntry* entry = sMapStore.LookupEntry(itr->first);
        if (!entry || entry->IsRaid() != isRaid || (!p->CanReset() && method != INSTANCE_RESET_GROUP_DISBAND))
        {
            ++itr;
            continue;
        }

        if (method == INSTANCE_RESET_ALL)
        {
            // the "reset all instances" method can only reset normal maps
            if (entry->map_type == MAP_RAID || diff == DUNGEON_DIFFICULTY_HEROIC)
            {
                ++itr;
                continue;
            }
        }

        bool isEmpty = true;
        // if the map is loaded, reset it
        Map* map = sMapMgr->FindMap(p->GetMapId(), p->GetInstanceId());
        if (map && map->IsDungeon() && !(method == INSTANCE_RESET_GROUP_DISBAND && !p->CanReset()))
        {
            if (p->CanReset())
                isEmpty = ((InstanceMap*)map)->Reset(method);
            else
                isEmpty = !map->HavePlayers();
        }

        if (SendMsgTo)
        {
            if (isEmpty)
                SendMsgTo->SendResetInstanceSuccess(p->GetMapId());
            else
                SendMsgTo->SendResetInstanceFailed(0, p->GetMapId());
        }

        if (isEmpty || method == INSTANCE_RESET_GROUP_DISBAND || method == INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // do not reset the instance, just unbind if others are permanently bound to it
            if (p->CanReset())
                p->DeleteFromDB();
            else
                CharacterDatabase.PExecute("DELETE FROM group_instance WHERE instance = '%u'", p->GetInstanceId());

            // i don't know for sure if hash_map iterators
            m_boundInstances[diff].erase(itr);
            itr = m_boundInstances[diff].begin();
            // this unloads the instance save unless online players are bound to it
            // (eg. permanent binds or GM solo binds)
            p->RemoveGroup(this);
        }
        else
            ++itr;
    }
}

InstanceGroupBind* Group::GetBoundInstance(Player* player)
{
    uint32 mapid = player->GetMapId();
    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    return GetBoundInstance(mapEntry);
}

InstanceGroupBind* Group::GetBoundInstance(Map* aMap)
{
    // Currently spawn numbering not different from map difficulty
    Difficulty difficulty = GetDifficulty(aMap->IsRaid());

    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(aMap->GetId(), difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(aMap->GetId());
    if (itr != m_boundInstances[difficulty].end())
        return &itr->second;
    else
        return NULL;
}

InstanceGroupBind* Group::GetBoundInstance(MapEntry const* mapEntry)
{
    if (!mapEntry)
        return NULL;

    Difficulty difficulty = GetDifficulty(mapEntry->IsRaid());

    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(mapEntry->MapID, difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapEntry->MapID);
    if (itr != m_boundInstances[difficulty].end())
        return &itr->second;
    else
        return NULL;
}

InstanceGroupBind* Group::BindToInstance(InstanceSave* save, bool permanent, bool load)
{
    if (!save || isBGGroup())
        return NULL;

    InstanceGroupBind& bind = m_boundInstances[save->GetDifficulty()][save->GetMapId()];
    if (!load && (!bind.save || permanent != bind.perm || save != bind.save))
        CharacterDatabase.PExecute("REPLACE INTO group_instance (guid, instance, permanent) VALUES (%u, %u, %u)", m_dbStoreId, save->GetInstanceId(), permanent);

    if (bind.save != save)
    {
        if (bind.save)
            bind.save->RemoveGroup(this);
        save->AddGroup(this);
    }

    bind.save = save;
    bind.perm = permanent;
    if (!load)
        sLog->outDebug(LOG_FILTER_MAPS, "Group::BindToInstance: Group (guid: %u, storage id: %u) is now bound to map %d, instance %d, difficulty %d",
        GUID_LOPART(GetGUID()), m_dbStoreId, save->GetMapId(), save->GetInstanceId(), save->GetDifficulty());

    return &bind;
}

void Group::UnbindInstance(uint32 mapid, uint8 difficulty, bool unload)
{
    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapid);
    if (itr != m_boundInstances[difficulty].end())
    {
        if (!unload)
            CharacterDatabase.PExecute("DELETE FROM group_instance WHERE guid=%u AND instance=%u", m_dbStoreId, itr->second.save->GetInstanceId());
        itr->second.save->RemoveGroup(this);                // save can become invalid
        m_boundInstances[difficulty].erase(itr);
    }
}

void Group::_homebindIfInstance(Player* player)
{
    if (player && !player->isGameMaster() && sMapStore.LookupEntry(player->GetMapId())->IsDungeon())
        player->m_InstanceValid = false;
}

void Group::BroadcastGroupUpdate(void)
{
    // FG: HACK: force flags update on group leave - for values update hack
    // -- not very efficient but safe
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* pp = ObjectAccessor::FindPlayer(citr->guid);
        if (pp && pp->IsInWorld())
        {
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_BYTES_2);
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_FACTIONTEMPLATE);
            sLog->outStaticDebug("-- Forced group value update for '%s'", pp->GetName());
        }
    }
}

void Group::ResetMaxEnchantingLevel()
{
    m_maxEnchantingLevel = 0;
    Player* pMember = NULL;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        pMember = ObjectAccessor::FindPlayer(citr->guid);
        if (pMember && m_maxEnchantingLevel < pMember->GetSkillValue(SKILL_ENCHANTING))
            m_maxEnchantingLevel = pMember->GetSkillValue(SKILL_ENCHANTING);
    }
}

void Group::SetLootMethod(LootMethod method)
{
    m_lootMethod = method;
}

void Group::SetLooterGuid(uint64 guid)
{
    m_looterGuid = guid;
}

void Group::SetLootThreshold(ItemQualities threshold)
{
    m_lootThreshold = threshold;
}

void Group::SetLfgRoles(uint64 guid, const uint8 roles)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    slot->roles = roles;
         SendUpdate();
}

bool Group::IsFull() const
{
    return isRaidGroup() ? (m_memberSlots.size() >= MAXRAIDSIZE) : (m_memberSlots.size() >= MAXGROUPSIZE);
}

bool Group::isLFGGroup() const
{
    return m_groupType & GROUPTYPE_LFG;
}

bool Group::isRaidGroup() const
{
    return m_groupType & GROUPTYPE_RAID;
}

bool Group::isBGGroup() const
{
    return m_bgGroup != NULL;
}

bool Group::IsCreated() const
{
    return GetMembersCount() > 0;
}

uint64 Group::GetLeaderGUID() const
{
    return m_leaderGuid;
}

uint64 Group::GetGUID() const
{
    return m_guid;
}

uint32 Group::GetLowGUID() const
{
    return GUID_LOPART(m_guid);
}

const char * Group::GetLeaderName() const
{
    return m_leaderName.c_str();
}

LootMethod Group::GetLootMethod() const
{
    return m_lootMethod;
}

uint64 Group::GetLooterGuid() const
{
    return m_looterGuid;
}

ItemQualities Group::GetLootThreshold() const
{
    return m_lootThreshold;
}

bool Group::IsMember(uint64 guid) const
{
    return _getMemberCSlot(guid) != m_memberSlots.end();
}

bool Group::IsLeader(uint64 guid) const
{
    return (GetLeaderGUID() == guid);
}

uint64 Group::GetMemberGUID(const std::string& name)
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->name == name)
            return itr->guid;
    return 0;
}

bool Group::IsAssistant(uint64 guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return false;
    return mslot->flags & MEMBER_FLAG_ASSISTANT;
}

bool Group::SameSubGroup(uint64 guid1, uint64 guid2) const
{
    member_citerator mslot2 = _getMemberCSlot(guid2);
    if (mslot2 == m_memberSlots.end())
       return false;
    return SameSubGroup(guid1, &*mslot2);
}

bool Group::SameSubGroup(uint64 guid1, MemberSlot const* slot2) const
{
    member_citerator mslot1 = _getMemberCSlot(guid1);
    if (mslot1 == m_memberSlots.end() || !slot2)
        return false;
    return (mslot1->group == slot2->group);
}

bool Group::HasFreeSlotSubGroup(uint8 subgroup) const
{
    return (m_subGroupsCounts && m_subGroupsCounts[subgroup] < MAXGROUPSIZE);
}

Group::MemberSlotList const& Group::GetMemberSlots() const
{
    return m_memberSlots;
}

GroupReference* Group::GetFirstMember()
{
    return m_memberMgr.getFirst();
}

uint32 Group::GetMembersCount() const
{
    return m_memberSlots.size();
}

uint8 Group::GetMemberGroup(uint64 guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
       return (MAX_RAID_SUBGROUPS+1);
    return mslot->group;
}

void Group::SetBattlegroundGroup(Battleground* bg)
{
    m_bgGroup = bg;
}

void Group::SetGroupMemberFlag(uint64 guid, bool apply, GroupMemberFlags flag)
{
    // Assistants, main assistants and main tanks are only available in raid groups
    if (!isRaidGroup())
       return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Do flag specific actions, e.g ensure uniqueness
    switch (flag) {
        case MEMBER_FLAG_MAINASSIST:
            RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINASSIST);         // Remove main assist flag from current if any.
            break;
        case MEMBER_FLAG_MAINTANK:
            RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINTANK);           // Remove main tank flag from current if any.
            break;
        case MEMBER_FLAG_ASSISTANT:
            break;
        default:
            return;                                                      // This should never happen
    }

    // Switch the actual flag
    ToggleGroupMemberFlag(slot, flag, apply);

    // Preserve the new setting in the db
    CharacterDatabase.PExecute("UPDATE group_member SET memberFlags='%u' WHERE memberGuid='%u'", slot->flags, GUID_LOPART(guid));

    // Broadcast the changes to the group
    SendUpdate();
}

Difficulty Group::GetDifficulty(bool isRaid) const
{
    return isRaid ? m_raidDifficulty : m_dungeonDifficulty;
}

Difficulty Group::GetDungeonDifficulty() const
{
    return m_dungeonDifficulty;
}

Difficulty Group::GetRaidDifficulty() const
{
    return m_raidDifficulty;
}

bool Group::isRollLootActive() const
{
    return !RollId.empty();
}

Group::Rolls::iterator Group::GetRoll(uint64 Guid)
{
    Rolls::iterator iter;
    for (iter=RollId.begin(); iter != RollId.end(); ++iter)
        if ((*iter)->itemGUID == Guid && (*iter)->isValid())
            return iter;
    return RollId.end();
}

void Group::LinkMember(GroupReference* pRef)
{
    m_memberMgr.insertFirst(pRef);
}

void Group::DelinkMember(GroupReference* /*pRef*/) const
{
}

Group::BoundInstancesMap& Group::GetBoundInstances(Difficulty difficulty)
{
    return m_boundInstances[difficulty];
}

void Group::_initRaidSubGroupsCounter()
{
    // Sub group counters initialization
    if (!m_subGroupsCounts)
        m_subGroupsCounts = new uint8[MAX_RAID_SUBGROUPS];

    memset((void*)m_subGroupsCounts, 0, (MAX_RAID_SUBGROUPS)*sizeof(uint8));

    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        ++m_subGroupsCounts[itr->group];
}

Group::member_citerator Group::_getMemberCSlot(uint64 Guid) const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

Group::member_witerator Group::_getMemberWSlot(uint64 Guid)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

void Group::SubGroupCounterIncrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        ++m_subGroupsCounts[subgroup];
}

void Group::SubGroupCounterDecrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        --m_subGroupsCounts[subgroup];
}

void Group::RemoveUniqueGroupMemberFlag(GroupMemberFlags flag)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->flags & flag)
            itr->flags &= ~flag;
}

void Group::ToggleGroupMemberFlag(member_witerator slot, uint8 flag, bool apply)
{
    if (apply)
        slot->flags |= flag;
    else
        slot->flags &= ~flag;
}

