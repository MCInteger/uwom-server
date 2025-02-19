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

/*
 * Comment: The event with the Living Mojos is not implemented, just is done that when one of the mojos around the boss take damage will make the boss enter in combat!
 */

#include "ScriptPCH.h"
#include "gundrak.h"

enum Spells
{
    SPELL_EMERGE                                  = 54850,
    SPELL_ELEMENTAL_SPAWN_EFFECT                  = 54888,
    SPELL_MOJO_VOLLEY                             = 54849,
    SPELL_SURGE_VISUAL                            = 54827,
    SPELL_MERGE                                   = 54878,
    SPELL_MIGHTY_BLOW                             = 54719,
    SPELL_SURGE                                   = 54801,
    SPELL_FREEZE_ANIM                             = 16245,
    SPELL_MOJO_PUDDLE                             = 55627,
    SPELL_MOJO_WAVE                               = 55626,
};

enum ColossusEvents
{
    EVENT_MIGHTY_BLOW   = 1,
};

enum ColossusActions
{
    ACTION_SUMMON_ELEMENTAL     = 1,
    ACTION_FREEZE_COLOSSUS      = 2,
    ACTION_UNFREEZE_COLOSSUS    = 3,
};

enum ColossusPhases
{
    COLOSSUS_PHASE_NORMAL                   = 1,
    COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON   = 2,
    COLOSSUS_PHASE_SECOND_ELEMENTAL_SUMMON  = 3
};

enum ColossusData
{
    DATA_COLOSSUS_PHASE = 1,
    DATA_INTRO_DONE     = 2
};

enum ElementalActions
{
    ACTION_RETURN_TO_COLOSSUS = 1
};

enum ElementalEvents
{
    EVENT_SURGE = 1
};

class boss_drakkari_colossus : public CreatureScript
{
    public:
        boss_drakkari_colossus() : CreatureScript("boss_drakkari_colossus") { }

        struct boss_drakkari_colossusAI : public BossAI
        {
            boss_drakkari_colossusAI(Creature* creature) : BossAI(creature, DATA_DRAKKARI_COLOSSUS_EVENT)
            {
                me->SetReactState(REACT_PASSIVE);
                introDone = false;
            }

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();

                if (GetData(DATA_INTRO_DONE))
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    me->RemoveAura(SPELL_FREEZE_ANIM);
                }

                //events.Reset(); -> done in _Reset();
                events.ScheduleEvent(EVENT_MIGHTY_BLOW, urand(10000, 30000));

                phase = COLOSSUS_PHASE_NORMAL;

                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                if (instance)
                {
                    instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, NOT_STARTED);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SURGE);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();

                me->RemoveAura(SPELL_FREEZE_ANIM);

                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                if (instance)
                    instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();

                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                if (instance)
                {
                    instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, DONE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SURGE);
                }
            }

            void JustReachedHome()
            {
                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                if (instance)
                    instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, FAIL);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SUMMON_ELEMENTAL:
                        DoCast(SPELL_EMERGE);
                        break;
                    case ACTION_FREEZE_COLOSSUS:
                        me->GetMotionMaster()->MoveIdle();

                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        DoCast(me, SPELL_FREEZE_ANIM);
                        break;
                    case ACTION_UNFREEZE_COLOSSUS:

                        if (me->GetReactState() == REACT_AGGRESSIVE)
                            return;

                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        me->RemoveAura(SPELL_FREEZE_ANIM);

                        me->SetInCombatWithZone();

                        if (me->getVictim())
                            me->GetMotionMaster()->MoveChase(me->getVictim(), 0, 0);

                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
                    damage = 0;

                if (phase == COLOSSUS_PHASE_NORMAL ||
                    phase == COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON)
                {
                    if (HealthBelowPct( phase == COLOSSUS_PHASE_NORMAL ? 50 : 5))
                    {
                        damage = 0;
                        phase = (phase == COLOSSUS_PHASE_NORMAL ? COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON : COLOSSUS_PHASE_SECOND_ELEMENTAL_SUMMON);
                        DoAction(ACTION_FREEZE_COLOSSUS);
                        DoAction(ACTION_SUMMON_ELEMENTAL);

                    }
                }
            }

            uint32 GetData(uint32 data)
            {
               if (data == DATA_COLOSSUS_PHASE)
                   return phase;
               else if (data == DATA_INTRO_DONE)
                   return introDone;

               return 0;
            }

            void SetData(uint32 type, uint32 data)
            {
                if (type == DATA_INTRO_DONE)
                    introDone = data;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MIGHTY_BLOW:
                            DoCast(me->getVictim(), SPELL_MIGHTY_BLOW);
                            events.ScheduleEvent(EVENT_MIGHTY_BLOW, urand(5000, 15000));
                            break;
                    }
                }

                if (me->GetReactState() == REACT_AGGRESSIVE)
                    DoMeleeAttackIfReady();
            }

            void JustSummoned(Creature* summon)
            {
               summon->SetInCombatWithZone();

               if (phase == COLOSSUS_PHASE_SECOND_ELEMENTAL_SUMMON)
                   summon->SetHealth(summon->GetMaxHealth() / 2);
            }

        private:
            uint8 phase;
            bool introDone;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_drakkari_colossusAI(creature);
        }
};

class boss_drakkari_elemental : public CreatureScript
{
    public:
        boss_drakkari_elemental() : CreatureScript("boss_drakkari_elemental") { }

        struct boss_drakkari_elementalAI : public ScriptedAI
        {
            boss_drakkari_elementalAI(Creature* creature) : ScriptedAI(creature)
            {
                DoCast(me, SPELL_ELEMENTAL_SPAWN_EFFECT);
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_SURGE, urand(5000, 15000));

                me->AddAura(SPELL_MOJO_VOLLEY, me);

                if (instance)
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SURGE);
            }

            void JustDied(Unit* killer)
            {
                if (killer == me)
                    return;

                if (instance)
                {
                    if (Creature* colossus = Unit::GetCreature(*me, instance->GetData64(DATA_DRAKKARI_COLOSSUS)))
                        killer->Kill(colossus);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SURGE);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SURGE:
                            DoCast(SPELL_SURGE_VISUAL);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_SURGE);
                            events.ScheduleEvent(EVENT_SURGE, urand(5000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

           void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_RETURN_TO_COLOSSUS:
                        DoCast(SPELL_SURGE_VISUAL);
                        if (instance)
                        {
                            if (Creature* colossus = Unit::GetCreature(*me, instance->GetData64(DATA_DRAKKARI_COLOSSUS)))
                                // what if the elemental is more than 80 yards from drakkari colossus ?
                                DoCast(colossus, SPELL_MERGE, true);
                        }
                        break;
                }
           }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                if (HealthBelowPct(50) && instance)
                    if (Creature* colossus = Unit::GetCreature(*me, instance->GetData64(DATA_DRAKKARI_COLOSSUS)))
                        if (colossus->AI()->GetData(DATA_COLOSSUS_PHASE) ==  COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON)
                        {
                            damage = 0;

                            // to prevent spell spaming
                            if (me->HasUnitState(UNIT_STAT_CHARGING))
                                return;

                            // not sure about this, the idea of this code is to prevent bug the elemental
                            // if it is not in a acceptable distance to cast the charge spell.
                            /*if (me->GetDistance(colossus) > 80.0f)
                            {
                                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                                    return;

                                me->GetMotionMaster()->MovePoint(0, colossus->GetPositionX(), colossus->GetPositionY(), colossus->GetPositionZ());
                                return;
                            }*/
                            DoAction(ACTION_RETURN_TO_COLOSSUS);
                        }
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon();
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_MERGE)
                    if (Creature* colossus = target->ToCreature())
                    {
                        colossus->AI()->DoAction(ACTION_UNFREEZE_COLOSSUS);
                        me->DespawnOrUnsummon();
                    }
            }

        private:
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_drakkari_elementalAI(creature);
        }
};

class npc_living_mojo : public CreatureScript
{
public:
    npc_living_mojo() : CreatureScript("npc_living_mojo") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_living_mojoAI (creature);
    }

    struct npc_living_mojoAI : public ScriptedAI
    {
        npc_living_mojoAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            mojoWaveTimer = 2*IN_MILLISECONDS;
            mojoPuddleTimer = 7*IN_MILLISECONDS;
        }

        void MoveMojos(Creature* boss)
        {
            std::list<Creature*> mojosList;
            boss->GetCreatureListWithEntryInGrid(mojosList, me->GetEntry(), 12.0f);
            if (!mojosList.empty())
                for (std::list<Creature*>::const_iterator itr = mojosList.begin(); itr != mojosList.end(); ++itr)
                    if (Creature* mojo = *itr)
                        mojo->GetMotionMaster()->MovePoint(1, boss->GetHomePosition().GetPositionX(), boss->GetHomePosition().GetPositionY(), boss->GetHomePosition().GetPositionZ());
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
                if (Creature* colossus = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_DRAKKARI_COLOSSUS) : 0))
                {
                    colossus->AI()->DoAction(ACTION_UNFREEZE_COLOSSUS);
                    if (!colossus->AI()->GetData(DATA_INTRO_DONE))
                        colossus->AI()->SetData(DATA_INTRO_DONE, true);
                    colossus->SetInCombatWithZone();
                    me->DespawnOrUnsummon();
                }
        }

        void AttackStart(Unit* attacker, float /*dist*/ = 0)
        {
            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                return;

            // we do this checks to see if the creature is one of the creatures that sorround the boss
            if (Creature* colossus = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_DRAKKARI_COLOSSUS) : 0))
            {
                Position homePosition;
                me->GetHomePosition().GetPosition(&homePosition);

                Position colossusHomePosition;
                colossus->GetHomePosition().GetPosition(&colossusHomePosition);

                float distance = homePosition.GetExactDist(colossusHomePosition.GetPositionX(), colossusHomePosition.GetPositionY(), colossusHomePosition.GetPositionZ());

                if (distance < 12.0f)
                {
                    MoveMojos(colossus);
                    me->SetReactState(REACT_PASSIVE);
                }
                else
                    ScriptedAI::AttackStart(attacker);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (mojoWaveTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MOJO_WAVE);
                mojoWaveTimer = 15*IN_MILLISECONDS;
            } else mojoWaveTimer -= diff;

            if (mojoPuddleTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MOJO_PUDDLE);
                mojoPuddleTimer = 18*IN_MILLISECONDS;
            } else mojoPuddleTimer -= diff;

            DoMeleeAttackIfReady();
        }
    private:
        InstanceScript* instance;
        uint32 mojoWaveTimer;
        uint32 mojoPuddleTimer;
    };
};

void AddSC_boss_drakkari_colossus()
{
    new boss_drakkari_colossus();
    new boss_drakkari_elemental();
    new npc_living_mojo();
}
