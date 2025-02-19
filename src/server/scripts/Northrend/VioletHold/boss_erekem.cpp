/*
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

#include "ScriptPCH.h"
#include "violet_hold.h"

enum Spells
{
    SPELL_BLOODLUST                             = 54516,
    SPELL_BREAK_BONDS                           = 59463,
    SPELL_CHAIN_HEAL                            = 54481,
    H_SPELL_CHAIN_HEAL                          = 59473,
    SPELL_EARTH_SHIELD                          = 54479,
    H_SPELL_EARTH_SHIELD                        = 59471,
    SPELL_EARTH_SHOCK                           = 54511,
    SPELL_LIGHTNING_BOLT                        = 53044,
    SPELL_STORMSTRIKE                           = 51876
};

enum Yells
{
    SAY_AGGRO                                   = -1608010,
    SAY_SLAY_1                                  = -1608011,
    SAY_SLAY_2                                  = -1608012,
    SAY_SLAY_3                                  = -1608013,
    SAY_DEATH                                   = -1608014,
    SAY_SPAWN                                   = -1608015,
    SAY_ADD_KILLED                              = -1608016,
    SAY_BOTH_ADDS_KILLED                        = -1608017
};

class boss_erekem : public CreatureScript
{
public:
    boss_erekem() : CreatureScript("boss_erekem") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_erekemAI (creature);
    }

    struct boss_erekemAI : public ScriptedAI
    {
        boss_erekemAI(Creature* c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        uint32 uiBloodlustTimer;
        uint32 uiChainHealTimer;
        uint32 uiEarthShockTimer;
        uint32 uiLightningBoltTimer;
        uint32 uiEarthShieldTimer;

        InstanceScript* instance;

        void Reset()
        {
            uiBloodlustTimer = 15000;
            uiChainHealTimer = 0;
            uiEarthShockTimer = urand(2000, 8000);
            uiLightningBoltTimer = urand(5000, 10000);
            uiEarthShieldTimer = 20000;
            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }

            if (Creature* pGuard1 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_1) : 0))
            {
                if (!pGuard1->isAlive())
                    pGuard1->Respawn();
            }
            if (Creature* pGuard2 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_2) : 0))
            {
                if (!pGuard2->isAlive())
                    pGuard2->Respawn();
            }
        }

        void AttackStart(Unit * who, float /*dist*/ = 0)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);

                if (Creature* pGuard1 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_1) : 0))
                {
                    pGuard1->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE|UNIT_FLAG_NON_ATTACKABLE);
                    if (!pGuard1->getVictim() && pGuard1->AI())
                        pGuard1->AI()->AttackStart(who);
                }
                if (Creature* pGuard2 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_2) : 0))
                {
                    pGuard2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE|UNIT_FLAG_NON_ATTACKABLE);
                    if (!pGuard2->getVictim() && pGuard2->AI())
                        pGuard2->AI()->AttackStart(who);
                }
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoCast(me, SPELL_EARTH_SHIELD);

            if (instance)
            {
                if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_EREKEM_CELL)))
                    if (pDoor->GetGoState() == GO_STATE_READY)
                    {
                        EnterEvadeMode();
                        return;
                    }

                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //spam stormstrike in hc mode if spawns are dead
            if (IsHeroic())
            {
                if (Creature* pGuard1 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_1) : 0))
                {
                    if (Creature* pGuard2 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_2) : 0))
                    {
                        if (!pGuard1->isAlive() && !pGuard2->isAlive())
                            DoCast(me->getVictim(), SPELL_STORMSTRIKE);
                    }
                }
            }

            if (uiEarthShieldTimer <= diff)
            {
                DoCast(me, SPELL_EARTH_SHIELD);
                uiEarthShieldTimer = 20000;
            } else uiEarthShieldTimer -= diff;

            if (uiChainHealTimer <= diff)
            {
                if (uint64 TargetGUID = GetChainHealTargetGUID())
                {
                    if (Creature* target = Unit::GetCreature(*me, TargetGUID))
                        DoCast(target, SPELL_CHAIN_HEAL);

                    //If one of the adds is dead spawn heals faster
                    Creature* pGuard1 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_1) : 0);
                    Creature* pGuard2 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_2) : 0);
                    uiChainHealTimer = ((pGuard1 && !pGuard1->isAlive()) || (pGuard2 && !pGuard2->isAlive()) ? 3000 : 8000) + rand()%3000;
                }
            } else uiChainHealTimer -= diff;

            if (uiBloodlustTimer <= diff)
            {
                DoCast(me, SPELL_BLOODLUST);
                uiBloodlustTimer = urand(35000, 45000);
            } else uiBloodlustTimer -= diff;

            if (uiEarthShockTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_EARTH_SHOCK);
                uiEarthShockTimer = urand(8000, 13000);
            } else uiEarthShockTimer -= diff;

            if (uiLightningBoltTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_LIGHTNING_BOLT);
                uiLightningBoltTimer = urand(18000, 24000);
            } else uiLightningBoltTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    instance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    instance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 13);
                }
            }
        }

        void KilledUnit(Unit* victim)
        {
            if (victim == me)
                return;
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3), me);
        }

        uint64 GetChainHealTargetGUID()
        {
            if (HealthBelowPct(85))
                return me->GetGUID();

            Creature* pGuard1 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_1) : 0);
            if (pGuard1 && pGuard1->isAlive() && !pGuard1->HealthAbovePct(75))
                return pGuard1->GetGUID();

            Creature* pGuard2 = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_EREKEM_GUARD_2) : 0);
            if (pGuard2 && pGuard2->isAlive() && !pGuard2->HealthAbovePct(75))
                return pGuard2->GetGUID();

            return 0;
        }
    };

};

enum GuardSpells
{
    SPELL_GUSHING_WOUND                   = 39215,
    SPELL_HOWLING_SCREECH                 = 54462,
    SPELL_STRIKE                          = 14516
};

class mob_erekem_guard : public CreatureScript
{
public:
    mob_erekem_guard() : CreatureScript("mob_erekem_guard") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_erekem_guardAI (creature);
    }

    struct mob_erekem_guardAI : public ScriptedAI
    {
        mob_erekem_guardAI(Creature* c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        uint32 uiGushingWoundTimer;
        uint32 uiHowlingScreechTimer;
        uint32 uiStrikeTimer;

        InstanceScript* instance;

        void Reset()
        {
            uiStrikeTimer = urand(4000, 8000);
            uiHowlingScreechTimer = urand(8000, 13000);
            uiGushingWoundTimer = urand(1000, 3000);
        }

        void AttackStart(Unit * who, float /*dist*/ = 0)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();

            if (uiStrikeTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_STRIKE);
                uiStrikeTimer = urand(4000, 8000);
            } else uiStrikeTimer -= diff;

            if (uiHowlingScreechTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_HOWLING_SCREECH);
                uiHowlingScreechTimer = urand(8000, 13000);
            } else uiHowlingScreechTimer -= diff;

            if (uiGushingWoundTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_GUSHING_WOUND);
                uiGushingWoundTimer = urand(7000, 12000);
            } else uiGushingWoundTimer -= diff;
        }
    };

};

void AddSC_boss_erekem()
{
    new boss_erekem();
    new mob_erekem_guard();
}
