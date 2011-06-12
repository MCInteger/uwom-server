/*
 * Copyright (C) 2009-2011 by WarHead - United Worlds of MaNGOS - http://www.uwom.de
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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

/* Script Data Start
SDName: Dalaran
SDAuthor: WarHead, MaXiMiUS
SD%Complete: 99%
SDComment: For what is 63990+63991? Same function but don't work correct...
SDCategory: Dalaran
Script Data End */

#include "ScriptPCH.h"

// NPC IDs: 29254,29255

/*******************************************************
 * npc_mageguard_dalaran
 *******************************************************/

enum SpellsAndTexts
{
    SPELL_EINDRINGLING_A    = 54028,
    SPELL_EINDRINGLING_H    = 54029,
    TEXT_A                  = -1000099,
    TEXT_H                  = -1000098,
};

class npc_mageguard_dalaran : public CreatureScript
{
public:
    npc_mageguard_dalaran() : CreatureScript("npc_mageguard_dalaran") { }

    struct npc_mageguard_dalaranAI : public Scripted_NoMovementAI
    {
        npc_mageguard_dalaranAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
        }

        void Reset()
        {
            me->SetOrientation(me->GetHomePosition().GetOrientation());
        }

        void EnterCombat(Unit* who)
        {
            return;
        }

        void AttackStart(Unit* who)
        {
            return;
        }

        void MoveInLineOfSight(Unit *who)
        {
            if (!who)
                return;

            Player* pwho = NULL;

            if (who->GetTypeId() == TYPEID_PLAYER)
                pwho = (Player*)who;

            if (!pwho || pwho->isGameMaster())
                return;

            if (me->GetDistance(pwho) >= 12.0f)
                return;

            switch(me->GetEntry())
            {
                case 29254: // Ally
                    if (!me->isInCombat() && pwho->GetTeam() == HORDE && !pwho->HasAura(SPELL_EINDRINGLING_A))
                    {
                    {
                        me->Yell(TEXT_A, LANG_UNIVERSAL, pwho->GetGUID());
                        DoCast(pwho, SPELL_EINDRINGLING_A);
                    }
                    }
                    break;
                case 29255: // Horde
                    if (!me->isInCombat() && pwho->GetTeam() == ALLIANCE && !pwho->HasAura(SPELL_EINDRINGLING_H))
                    {
                    {
                        me->Yell(TEXT_H, LANG_UNIVERSAL, pwho->GetGUID());
                        DoCast(pwho, SPELL_EINDRINGLING_H);
                    }
                    }
                    break;
            }
            EnterEvadeMode();
            return;
        }

        void UpdateAI(const uint32 diff) {}
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_mageguard_dalaranAI(creature);
    }
};

/*######
## npc_hira_snowdawn
######*/

enum eHiraSnowdawn
{
    SPELL_COLD_WEATHER_FLYING                   = 54197
};

#define GOSSIP_TEXT_TRAIN_HIRA "I seek training to ride a steed."

class npc_hira_snowdawn : public CreatureScript
{
public:
    npc_hira_snowdawn() : CreatureScript("npc_hira_snowdawn") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (!pCreature->isVendor() || !pCreature->isTrainer())
            return false;

        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_TRAIN_HIRA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

        if (pPlayer->getLevel() >= 80 && pPlayer->HasSpell(SPELL_COLD_WEATHER_FLYING))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_TRAIN)
            pPlayer->GetSession()->SendTrainerList(pCreature->GetGUID());

        if (uiAction == GOSSIP_ACTION_TRADE)
            pPlayer->GetSession()->SendListInventory(pCreature->GetGUID());

        return true;
    }
};

void AddSC_dalaran()
{
    new npc_mageguard_dalaran;
    new npc_hira_snowdawn;
}
