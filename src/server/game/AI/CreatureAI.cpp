/*
 * Copyright (c) 2008-2011 by WarHead - United Worlds of MaNGOS - http://www.uwom.de
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

#include "CreatureAI.h"
#include "CreatureAIImpl.h"
#include "Creature.h"
#include "World.h"
#include "SpellMgr.h"
#include "Vehicle.h"
#include "Log.h"
#include "MapReference.h"
#include "Player.h"

//Disable CreatureAI when charmed
void CreatureAI::OnCharmed(bool /*apply*/)
{
    //me->IsAIEnabled = !apply;*/
    me->NeedChangeAI = true;
    me->IsAIEnabled = false;
}

AISpellInfoType* UnitAI::AISpellInfo;
AISpellInfoType* GetAISpellInfo(uint32 i) { return &CreatureAI::AISpellInfo[i]; }

void CreatureAI::Talk(uint8 id, uint64 WhisperGuid)
{
    sCreatureTextMgr->SendChat(me, id, WhisperGuid);
}

void CreatureAI::DoZoneInCombat(Creature * creature, float range)
{
    if (!creature || creature->IsInWorld())
        creature = me;

    if (!creature->CanHaveThreatList())
        return;

    if (!creature->HasReactState(REACT_PASSIVE) && !creature->getVictim())
    {
        if (Unit * target = creature->SelectNearestTarget(range))
            creature->AI()->AttackStart(target);
        else if (creature->isSummon())
        {
            if (Unit * summoner = creature->ToTempSummon()->GetSummoner())
            {
                Unit * target = summoner->getAttackerForHelper();

                if (!target && summoner->CanHaveThreatList() && !summoner->getThreatManager().isThreatListEmpty())
                    target = summoner->getThreatManager().getHostilTarget();
                if (target && (creature->IsFriendlyTo(summoner) || creature->IsHostileTo(target)))
                    creature->AI()->AttackStart(target);
            }
        }
    }

    if (!creature->HasReactState(REACT_PASSIVE) && !creature->getVictim())
    {
        sLog->outError("DoZoneInCombat called for creature that has empty threat list (creature entry = %u)", creature->GetEntry());
        return;
    }

    Map * map = creature->GetMap();
    Map::PlayerList const & PlList = map->GetPlayers();

    if (PlList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator itr = PlList.begin(); itr != PlList.end(); ++itr)
        if (Player * pl = itr->getSource())
            if (pl->isAlive() && pl->IsInWorld())
            {
                if (pl->isGameMaster())
                    continue;

                if (creature->IsWithinDistInMap(pl, range))
                {
                    creature->SetInCombatWith(pl);
                    pl->SetInCombatWith(creature);
                    creature->AddThreat(pl, 0.0f);
                }
            }
}

// scripts does not take care about MoveInLineOfSight loops
// MoveInLineOfSight can be called inside another MoveInLineOfSight and cause stack overflow
void CreatureAI::MoveInLineOfSight_Safe(Unit* who)
{
    if (m_MoveInLineOfSight_locked == true)
        return;
    m_MoveInLineOfSight_locked = true;
    MoveInLineOfSight(who);
    m_MoveInLineOfSight_locked = false;
}

void CreatureAI::MoveInLineOfSight(Unit* who)
{
    if (me->getVictim())
        return;

    if (me->GetCreatureType() == CREATURE_TYPE_NON_COMBAT_PET) // non-combat pets should just stand there and look good;)
        return;

    if (me->canStartAttack(who, false))
        AttackStart(who);
    //else if (who->getVictim() && me->IsFriendlyTo(who)
    //    && me->IsWithinDistInMap(who, sWorld->getIntConfig(CONFIG_CREATURE_FAMILY_ASSISTANCE_RADIUS))
    //    && me->canStartAttack(who->getVictim(), true)) // TODO: if we use true, it will not attack it when it arrives
    //    me->GetMotionMaster()->MoveChase(who->getVictim());
}

void CreatureAI::EnterEvadeMode()
{
    if (!_EnterEvadeMode())
        return;

    sLog->outDebug(LOG_FILTER_UNITS, "Creature %u enters evade mode.", me->GetEntry());

    if (!me->GetVehicle()) // otherwise me will be in evade mode forever
    {
        if (Unit* owner = me->GetCharmerOrOwner())
        {
            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle(), MOTION_SLOT_ACTIVE);
        }
        else
            me->GetMotionMaster()->MoveTargetedHome();
    }

    Reset();

    if (me->IsVehicle()) // use the same sequence of addtoworld, aireset may remove all summons!
        me->GetVehicleKit()->Reset(true);
}

/*void CreatureAI::AttackedBy(Unit* attacker)
{
    if (!me->getVictim())
        AttackStart(attacker);
}*/
