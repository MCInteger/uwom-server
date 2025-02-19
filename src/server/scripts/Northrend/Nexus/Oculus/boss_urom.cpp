/*
 * Copyright (C) 2008-2011 by WarHead - United Worlds of MaNGOS - http://www.uwom.de
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

/* ScriptData
SDName: Urom
SD%Complete: 80
SDComment: Is not working SPELL_ARCANE_SHIELD. SPELL_FROSTBOMB has some issues, the damage aura should not stack.
SDCategory: Instance Script
EndScriptData */

#include "ScriptPCH.h"
#include "oculus.h"

enum Spells
{

    SPELL_ARCANE_SHIELD                 = 53813, //Dummy --> Channeled, shields the caster from damage.
    SPELL_EMPOWERED_ARCANE_EXPLOSION    = 51110,
    SPELL_EMPOWERED_ARCANE_EXPLOSION_2  = 59377,
    SPELL_FROSTBOMB                     = 51103, //Urom throws a bomb, hitting its target with the highest aggro which inflict directly 650 frost damage and drops a frost zone on the ground. This zone deals 650 frost damage per second and reduce the movement speed by 35%. Lasts 1 minute.
    SPELL_SUMMON_MENAGERIE              = 50476, //Summons an assortment of creatures and teleports the caster to safety.
    SPELL_SUMMON_MENAGERIE_2            = 50495,
    SPELL_SUMMON_MENAGERIE_3            = 50496,
    SPELL_TELEPORT                      = 51112, //Teleports to the center of Oculus
    SPELL_TIME_BOMB                     = 51121, //Deals arcane damage to a random player, and after 6 seconds, deals zone damage to nearby equal to the health missing of the target afflicted by the debuff.
    SPELL_TIME_BOMB_2                   = 59376
};

enum Yells
{
    SAY_AGGRO_1     = -1578000,
    SAY_AGGRO_2     = -1578001,
    SAY_AGGRO_3     = -1578002,
    SAY_AGGRO_4     = -1578003,
    SAY_TELEPORT    = -1578004
};

enum eCreature
{
    NPC_PHANTASMAL_CLOUDSCRAPER = 27645,
    NPC_PHANTASMAL_MAMMOTH      = 27642,
    NPC_PHANTASMAL_WOLF         = 27644,

    NPC_PHANTASMAL_AIR          = 27650,
    NPC_PHANTASMAL_FIRE         = 27651,
    NPC_PHANTASMAL_WATER        = 27653,

    NPC_PHANTASMAL_MURLOC       = 27649,
    NPC_PHANTASMAL_NAGAL        = 27648,
    NPC_PHANTASMAL_OGRE         = 27647
};

struct Summons
{
    uint32 entry[4];
};

static Summons Group[]=
{
    { {NPC_PHANTASMAL_CLOUDSCRAPER, NPC_PHANTASMAL_CLOUDSCRAPER, NPC_PHANTASMAL_MAMMOTH, NPC_PHANTASMAL_WOLF} },
    { {NPC_PHANTASMAL_AIR, NPC_PHANTASMAL_AIR, NPC_PHANTASMAL_WATER, NPC_PHANTASMAL_FIRE} },
    { {NPC_PHANTASMAL_OGRE, NPC_PHANTASMAL_OGRE, NPC_PHANTASMAL_NAGAL, NPC_PHANTASMAL_MURLOC} }
};

static uint32 TeleportSpells[]=
{
    SPELL_SUMMON_MENAGERIE, SPELL_SUMMON_MENAGERIE_2, SPELL_SUMMON_MENAGERIE_3
};

static int32 SayAggro[]=
{
    SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3, SAY_AGGRO_4
};

class boss_urom : public CreatureScript
{
public:
    boss_urom() : CreatureScript("boss_urom") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_uromAI (creature);
    }

    struct boss_uromAI : public BossAI
    {
        boss_uromAI(Creature* creature) : BossAI(creature, DATA_UROM_EVENT)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void Reset()
        {
            if (!me->isAlive())
                return;

            me->ResetLootMode();
            events.Reset();

            if (instance)
            {
                instance->SetBossState(DATA_UROM_EVENT, NOT_STARTED);
                if (instance->GetData(DATA_UROM_PLATAFORM) == 0)
                    for (uint8 i = 0; i < 3; i++)
                        group[i] = 0;
            }

            x = 0.0f;
            y = 0.0f;

            canCast = false;
            canGoBack = false;

            me->GetMotionMaster()->MoveIdle();

            teleportTimer = urand(30000, 35000);
            arcaneExplosionTimer = 9000;
            castArcaneExplosionTimer = 2000;
            frostBombTimer = urand(5000, 8000);
            timeBombTimer = urand(20000, 25000);
        }

        bool UromErlaubt()
        {
            if (instance && instance->GetBossState(DATA_VAROS) != DONE && !me->HasAura(SPELL_ARCANE_SHIELD))
            {
                DoCast(SPELL_ARCANE_SHIELD);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
            else if (instance->GetBossState(DATA_VAROS) == DONE)
            {
                if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                {
                    me->RemoveAurasDueToSpell(SPELL_ARCANE_SHIELD);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
                return true;
            }
            return false;
        }

        void MoveInLineOfSight(Unit * who)
        {
            if (!instance || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (UromErlaubt() && who && me->GetDistance(who) <= 20.0f)
            {
                SetGroups();
                SummonGroups();
                CastTeleport();

                if (instance->GetData(DATA_UROM_PLATAFORM) != 3)
                    instance->SetData(DATA_UROM_PLATAFORM, instance->GetData(DATA_UROM_PLATAFORM)+1);
            }
            ScriptedAI::MoveInLineOfSight(who);
        }

        void EnterCombat(Unit * /*who*/)
        {
            if (!instance || instance->GetData(DATA_UROM_PLATAFORM) < 2)
                return;

            _EnterCombat();
        }

        void AttackStart(Unit * who, float /*dist*/ = 0)
        {
            if (!who)
                return;

            if (me->GetPositionZ() > 518.63f)
                DoStartNoMovement(who);

            if (me->GetPositionZ() < 518.63f)
            {
                if (me->Attack(who, true))
                {
                    DoScriptText(SayAggro[3], me);

                    me->SetInCombatWith(who);
                    who->SetInCombatWith(me);

                    me->GetMotionMaster()->MoveChase(who, 0, 0);
                }
            }
        }

        void SetGroups()
        {
            if (!instance || instance->GetData(DATA_UROM_PLATAFORM) != 0)
                return;

            while (group[0] == group[1] || group[0] == group[2] || group[1] == group[2])
            {
                for (uint8 i = 0; i < 3; i++)
                    group[i] = urand(0, 2);
            }
        }

        void SetPosition(uint8 i)
        {
            switch(i)
            {
                case 0:
                    x = me->GetPositionX() + 4;
                    y = me->GetPositionY() - 4;
                    break;
                case 1:
                    x = me->GetPositionX() + 4;
                    y = me->GetPositionY() + 4;
                    break;
                case 2:
                    x = me->GetPositionX() - 4;
                    y = me->GetPositionY() + 4;
                    break;
                case 3:
                    x = me->GetPositionX() - 4;
                    y = me->GetPositionY() - 4;
                    break;
                default:
                    break;
            }
        }

        void SummonGroups()
        {
            if (!instance || instance->GetData(DATA_UROM_PLATAFORM) > 2)
                return;

            for (uint8 i=0; i<4; ++i)
            {
                SetPosition(i);
                if (TempSummon * ts = me->SummonCreature(Group[group[instance->GetData(DATA_UROM_PLATAFORM)]].entry[i], x, y, me->GetPositionZ(), me->GetOrientation()))
                    if (Player * pl = ts->FindNearestPlayer(100.0f))
                        ts->AI()->AttackStart(pl);
            }
        }

        void CastTeleport()
        {
            if (!instance || instance->GetData(DATA_UROM_PLATAFORM) > 2)
                return;

            DoScriptText(SayAggro[instance->GetData(DATA_UROM_PLATAFORM)], me);
            DoCast(TeleportSpells[instance->GetData(DATA_UROM_PLATAFORM)]);
        }

        void JustDied(Unit * /*killer*/)
        {
            _JustDied();
        }

        void SpellHit(Unit * /*pCaster*/, const SpellInfo * pSpell)
        {
            switch(pSpell->Id)
            {
                case SPELL_SUMMON_MENAGERIE:
                    me->SetHomePosition(968.66f, 1042.53f, 527.32f, 0.077f);
                    EnterEvadeMode();
                    break;
                case SPELL_SUMMON_MENAGERIE_2:
                    me->SetHomePosition(1164.02f, 1170.85f, 527.321f, 3.66f);
                    EnterEvadeMode();
                    break;
                case SPELL_SUMMON_MENAGERIE_3:
                    me->SetHomePosition(1118.31f, 1080.377f, 508.361f, 4.25f);
                    EnterEvadeMode();
                    break;
                case SPELL_TELEPORT:
                    me->SetFlying(true); // with out it the npc will fall down while is casting
                    canCast = true;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UromErlaubt() || !UpdateVictim())
                return;

            if (!instance || instance->GetData(DATA_UROM_PLATAFORM) < 2)
                return;

            if (teleportTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoScriptText(SAY_TELEPORT, me);
                me->GetMotionMaster()->MoveIdle();
                DoCast(SPELL_TELEPORT);
                teleportTimer = 15000;
            }
            else
                teleportTimer -= uiDiff;

            if (canCast && !me->FindCurrentSpellBySpellId(SPELL_EMPOWERED_ARCANE_EXPLOSION))
            {
                if (castArcaneExplosionTimer <= uiDiff)
                {
                    canCast = false;
                    canGoBack = true;
                    DoCastAOE(SPELL_EMPOWERED_ARCANE_EXPLOSION);
                    castArcaneExplosionTimer = 2000;
                }
                else
                    castArcaneExplosionTimer -= uiDiff;
            }

            if (canGoBack)
            {
                if (arcaneExplosionTimer <= uiDiff)
                {
                    Position pPos;
                    me->getVictim()->GetPosition(&pPos);

                    me->NearTeleportTo(pPos.GetPositionX(), pPos.GetPositionY(), pPos.GetPositionZ(), pPos.GetOrientation());
                    me->SetFlying(false);
                    me->GetMotionMaster()->MoveChase(me->getVictim());

                    canCast = false;
                    canGoBack = false;
                    arcaneExplosionTimer = 9000;
                }
                else
                    arcaneExplosionTimer -= uiDiff;
            }

            if (!me->IsNonMeleeSpellCasted(false, true, true))
            {
                if (frostBombTimer <= uiDiff)
                {
                    DoCastVictim(SPELL_FROSTBOMB);
                    frostBombTimer = urand(5000, 8000);
                }
                else
                    frostBombTimer -= uiDiff;

                if (timeBombTimer <= uiDiff)
                {
                    if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_TIME_BOMB);
                    timeBombTimer = urand(20000, 25000);
                }
                else
                    timeBombTimer -= uiDiff;
            }
            DoMeleeAttackIfReady();
        }
    private:
        float x, y;

        bool canCast;
        bool canGoBack;

        uint8 group[3];

        uint32 teleportTimer;
        uint32 arcaneExplosionTimer;
        uint32 castArcaneExplosionTimer;
        uint32 frostBombTimer;
        uint32 timeBombTimer;
    };
};

void AddSC_boss_urom()
{
    new boss_urom();
}
