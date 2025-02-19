/*
 * Copyright (C) 2008-2011 by WarHead - United Worlds of MaNGOS - http://www.uwom.de
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Ragnaros
SD%Complete: 95
SDComment: some spells doesnt work correctly
SDCategory: Molten Core
EndScriptData */

#include "ScriptPCH.h"
#include "molten_core.h"
#include <AccountMgr.h>

enum Texts
{
    SAY_REINFORCEMENTS1 = -1409013,
    SAY_REINFORCEMENTS2 = -1409014,
    SAY_HAND            = -1409015,
    SAY_WRATH           = -1409016,
    SAY_KILL            = -1409017,
    SAY_MAGMABURST      = -1409018,
    SAY_SUMMON_MAJ      = -1409008,
    SAY_ARRIVAL1_RAG    = -1409009,
    SAY_ARRIVAL2_MAJ    = -1409010,
    SAY_ARRIVAL3_RAG    = -1409011,
    SAY_ARRIVAL5_RAG    = -1409012,
};

enum Spells
{
    SPELL_HAND_OF_RAGNAROS      = 19780,
    SPELL_WRATH_OF_RAGNAROS     = 20566,
    SPELL_LAVA_BURST            = 21158,
    SPELL_MAGMA_BLAST           = 20565,                   // Ranged attack
    SPELL_SONS_OF_FLAME_DUMMY   = 21108,                   // Server side effect
    SPELL_RAGSUBMERGE           = 21107,                   // Stealth aura
    SPELL_RAGEMERGE             = 20568,
    SPELL_MELT_WEAPON           = 21388,
    SPELL_ELEMENTAL_FIRE        = 20564,
    SPELL_ERRUPTION             = 17731,

    SPELL_ENRAGE                = 26662,
    SPELL_FEURIGE_EINAESCHERUNG = 74562,
    SPELL_GROSSBRAND            = 74456
};

enum Events
{
    EVENT_ERUPTION = 1,
    EVENT_WRATH_OF_RAGNAROS,
    EVENT_HAND_OF_RAGNAROS,
    EVENT_LAVA_BURST,
    EVENT_ELEMENTAL_FIRE,
    EVENT_MAGMA_BLAST,
    EVENT_SUBMERGE,

    EVENT_INTRO_1,
    EVENT_INTRO_2,
    EVENT_INTRO_3,
    EVENT_INTRO_4,
    EVENT_INTRO_5,

    EVENT_ENRAGE,
    EVENT_FEURIGE_EINAESCHERUNG,
    EVENT_GROSSBRAND
};

class boss_ragnaros_outdoor : public CreatureScript
{
    public:
        boss_ragnaros_outdoor() : CreatureScript("boss_ragnaros_outdoor") { }

        struct boss_ragnaros_outdoorAI : public BossAI
        {
            boss_ragnaros_outdoorAI(Creature * pCreature) : BossAI(pCreature, BOSS_RAGNAROS)
            {
                _introState = 0;
            }

            void Reset()
            {
                _emergeTimer = 15 * IN_MILLISECONDS;
                _hasYelledMagmaBurst = false;
                _hasSubmergedOnce = false;
                _isBanished = false;

                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                me->SetReactState(REACT_AGGRESSIVE);

                BossAI::Reset();
            }

            void MoveInLineOfSight(Unit * who)
            {
                if (!who || me->getVictim())
                    return;

                Unit * pTarget = who;
                // Keine NPCs angreifen, die nicht zu einem Spieler gehören!
                if (pTarget->GetTypeId() == TYPEID_UNIT && !pTarget->GetOwner())
                    return;

                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                    if (!AccountMgr::IsPlayerAccount(pTarget->ToPlayer()->GetSession()->GetSecurity())) // Nur Spieler angreifen, die keine GMs sind!
                        return;

                if (me->canStartAttack(pTarget, true))
                    AttackStart(pTarget);
            }

            void EnterCombat(Unit * victim)
            {
                BossAI::EnterCombat(victim);

                DoZoneInCombat(me, true);

                events.ScheduleEvent(EVENT_ERUPTION, 15 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_WRATH_OF_RAGNAROS, SEKUNDEN_30);
                events.ScheduleEvent(EVENT_HAND_OF_RAGNAROS, 25 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_LAVA_BURST, SEKUNDEN_10);
                events.ScheduleEvent(EVENT_ELEMENTAL_FIRE, 3 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_MAGMA_BLAST, 2 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_SUBMERGE, 45 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_ENRAGE, MINUTEN_10);
                events.ScheduleEvent(EVENT_FEURIGE_EINAESCHERUNG, SEKUNDEN_10);
                events.ScheduleEvent(EVENT_GROSSBRAND, SEKUNDEN_30);
            }

            void KilledUnit(Unit * victim)
            {
                if (victim && victim->GetTypeId() == TYPEID_PLAYER)
                    if (urand(0, 4) == 2)
                        DoScriptText(SAY_KILL, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!_introState)
                {
                    me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    _introState = 1;
                }
                else
                {
                    if (_isBanished && _emergeTimer <= diff)
                    {
                        //Become unbanished again
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->setFaction(14);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                        if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            AttackStart(target);
                        //DoCast(me, SPELL_RAGEMERGE); //"phase spells" didnt worked correctly so Ive commented them and wrote solution witch doesnt need core support
                        _isBanished = false;
                    }
                    else if (_isBanished)
                    {
                        _emergeTimer -= diff;
                        //Do nothing while banished
                        return;
                    }

                    //Return since we have no target
                    if (!UpdateVictim())
                        return;

                    events.Update(diff);

                    if (me->HasUnitState(UNIT_STAT_CASTING))
                        return;

                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                            case EVENT_ERUPTION:
                                DoCastVictim(SPELL_ERRUPTION);
                                events.ScheduleEvent(EVENT_ERUPTION, urand(SEKUNDEN_20, SEKUNDEN_30));
                                break;
                            case EVENT_WRATH_OF_RAGNAROS:
                                DoCastVictim(SPELL_WRATH_OF_RAGNAROS);
                                if (urand(0, 1))
                                    DoScriptText(SAY_WRATH, me);
                                events.ScheduleEvent(EVENT_WRATH_OF_RAGNAROS, 25 * IN_MILLISECONDS);
                                break;
                            case EVENT_HAND_OF_RAGNAROS:
                                DoCast(me, SPELL_HAND_OF_RAGNAROS);
                                if (urand(0, 1))
                                    DoScriptText(SAY_HAND, me);
                                events.ScheduleEvent(EVENT_HAND_OF_RAGNAROS, SEKUNDEN_20);
                                break;
                            case EVENT_LAVA_BURST:
                                DoCastVictim(SPELL_LAVA_BURST);
                                events.ScheduleEvent(EVENT_LAVA_BURST, SEKUNDEN_10);
                                break;
                            case EVENT_ELEMENTAL_FIRE:
                                DoCastVictim(SPELL_ELEMENTAL_FIRE);
                                events.ScheduleEvent(EVENT_ELEMENTAL_FIRE, urand(SEKUNDEN_10, 14 * IN_MILLISECONDS));
                                break;
                            case EVENT_MAGMA_BLAST:
                                if (me->IsWithinMeleeRange(me->getVictim()))
                                {
                                    DoCastVictim(SPELL_MAGMA_BLAST);
                                    if (!_hasYelledMagmaBurst)
                                    {
                                        //Say our dialog
                                        DoScriptText(SAY_MAGMABURST, me);
                                        _hasYelledMagmaBurst = true;
                                    }
                                }
                                events.ScheduleEvent(EVENT_MAGMA_BLAST, 2500);
                                break;
                            case EVENT_SUBMERGE:
                                if (!_isBanished)
                                {
                                    //Creature spawning and ragnaros becomming unattackable
                                    //is not very well supported in the core //no it really isnt
                                    //so added normaly spawning and banish workaround and attack again after 90 secs.
                                    me->AttackStop();
                                    me->SetReactState(REACT_PASSIVE);
                                    me->InterruptNonMeleeSpells(false);
                                    DoResetThreat();
                                    //Root self
                                    //DoCast(me, 23973);
                                    me->setFaction(35);
                                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
                                    me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);

                                    if (!_hasSubmergedOnce)
                                    {
                                        DoScriptText(SAY_REINFORCEMENTS1, me);
                                        _hasSubmergedOnce = true;
                                    }
                                    else
                                        DoScriptText(SAY_REINFORCEMENTS2, me);

                                    _isBanished = true;
                                    //DoCast(me, SPELL_RAGSUBMERGE);
                                    _emergeTimer = 20000;

                                    for (uint8 i=0; i<16; ++i)
                                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                            if (Creature* pSummoned = me->SummonCreature(12143, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 900000))
                                                pSummoned->AI()->AttackStart(pTarget);
                                }
                                events.ScheduleEvent(EVENT_SUBMERGE, 45 * IN_MILLISECONDS);
                                break;
                            case EVENT_ENRAGE:
                                DoCast(SPELL_ENRAGE);
                                break;
                            case EVENT_FEURIGE_EINAESCHERUNG:
                                if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                                    DoCast(target, SPELL_FEURIGE_EINAESCHERUNG);
                                events.RescheduleEvent(EVENT_FEURIGE_EINAESCHERUNG, urand(SEKUNDEN_20, SEKUNDEN_30));
                                break;
                            case EVENT_GROSSBRAND:
                                for (uint8 i=0; i<4; ++i)
                                    if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                                        DoCast(target, SPELL_GROSSBRAND);
                                events.RescheduleEvent(EVENT_GROSSBRAND, urand(SEKUNDEN_30, 45 * IN_MILLISECONDS));
                                break;
                        }
                    }
                    DoMeleeAttackIfReady();
                }
            }
        private:
            uint32 _emergeTimer;
            uint8 _introState;
            bool _hasYelledMagmaBurst;
            bool _hasSubmergedOnce;
            bool _isBanished;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_ragnaros_outdoorAI(pCreature);
        }
};

class boss_ragnaros : public CreatureScript
{
    public:
        boss_ragnaros() : CreatureScript("boss_ragnaros") { }

        struct boss_ragnarosAI : public BossAI
        {
            boss_ragnarosAI(Creature* creature) : BossAI(creature, BOSS_RAGNAROS)
            {
                _introState = 0;
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void Reset()
            {
                BossAI::Reset();
                _emergeTimer = 90000;
                _hasYelledMagmaBurst = false;
                _hasSubmergedOnce = false;
                _isBanished = false;
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            }

            void EnterCombat(Unit* victim)
            {
                BossAI::EnterCombat(victim);
                events.ScheduleEvent(EVENT_ERUPTION, 15000);
                events.ScheduleEvent(EVENT_WRATH_OF_RAGNAROS, 30000);
                events.ScheduleEvent(EVENT_HAND_OF_RAGNAROS, 25000);
                events.ScheduleEvent(EVENT_LAVA_BURST, 10000);
                events.ScheduleEvent(EVENT_ELEMENTAL_FIRE, 3000);
                events.ScheduleEvent(EVENT_MAGMA_BLAST, 2000);
                events.ScheduleEvent(EVENT_SUBMERGE, 180000);
            }

            void KilledUnit(Unit* /*victim*/)
            {
                if (urand(0, 99 < 25))
                    DoScriptText(SAY_KILL, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (_introState != 2)
                {
                    if (!_introState)
                    {
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                        events.ScheduleEvent(EVENT_INTRO_1, 4000);
                        events.ScheduleEvent(EVENT_INTRO_2, 23000);
                        events.ScheduleEvent(EVENT_INTRO_3, 42000);
                        events.ScheduleEvent(EVENT_INTRO_4, 43000);
                        events.ScheduleEvent(EVENT_INTRO_5, 53000);
                        _introState = 1;
                    }

                    events.Update(diff);

                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                        case EVENT_INTRO_1:
                            DoScriptText(SAY_ARRIVAL1_RAG, me);
                            break;
                        case EVENT_INTRO_2:
                            DoScriptText(SAY_ARRIVAL3_RAG, me);
                            break;
                        case EVENT_INTRO_3:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);
                            break;
                        case EVENT_INTRO_4:
                            DoScriptText(SAY_ARRIVAL5_RAG, me);
                            if (instance)
                                if (Creature* executus = Unit::GetCreature(*me, instance->GetData64(BOSS_MAJORDOMO_EXECUTUS)))
                                    me->Kill(executus);
                            break;
                        case EVENT_INTRO_5:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            _introState = 2;
                            break;
                        default:
                            break;
                        }
                    }
                }
                else
                {
                    if (instance)
                    {
                        if (_isBanished && ((_emergeTimer <= diff) || (instance->GetData(DATA_RAGNAROS_ADDS)) > 8))
                        {
                            //Become unbanished again
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->setFaction(14);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                AttackStart(target);
                            instance->SetData(DATA_RAGNAROS_ADDS, 0);

                            //DoCast(me, SPELL_RAGEMERGE); //"phase spells" didnt worked correctly so Ive commented them and wrote solution witch doesnt need core support
                            _isBanished = false;
                        }
                        else if (_isBanished)
                        {
                            _emergeTimer -= diff;
                            //Do nothing while banished
                            return;
                        }
                    }

                    //Return since we have no target
                    if (!UpdateVictim())
                        return;

                    events.Update(diff);

                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                            case EVENT_ERUPTION:
                                DoCastVictim(SPELL_ERRUPTION);
                                events.ScheduleEvent(EVENT_ERUPTION, urand(20000, 45000));
                                break;
                            case EVENT_WRATH_OF_RAGNAROS:
                                DoCastVictim(SPELL_WRATH_OF_RAGNAROS);
                                if (urand(0, 1))
                                    DoScriptText(SAY_WRATH, me);
                                events.ScheduleEvent(EVENT_WRATH_OF_RAGNAROS, 25000);
                                break;
                            case EVENT_HAND_OF_RAGNAROS:
                                DoCast(me, SPELL_HAND_OF_RAGNAROS);
                                if (urand(0, 1))
                                    DoScriptText(SAY_HAND, me);
                                events.ScheduleEvent(EVENT_HAND_OF_RAGNAROS, 20000);
                                break;
                            case EVENT_LAVA_BURST:
                                DoCastVictim(SPELL_LAVA_BURST);
                                events.ScheduleEvent(EVENT_LAVA_BURST, 10000);
                                break;
                            case EVENT_ELEMENTAL_FIRE:
                                DoCastVictim(SPELL_ELEMENTAL_FIRE);
                                events.ScheduleEvent(EVENT_ELEMENTAL_FIRE, urand(10000, 14000));
                                break;
                            case EVENT_MAGMA_BLAST:
                                if (me->IsWithinMeleeRange(me->getVictim()))
                                {
                                    DoCastVictim(SPELL_MAGMA_BLAST);
                                    if (!_hasYelledMagmaBurst)
                                    {
                                        //Say our dialog
                                        DoScriptText(SAY_MAGMABURST, me);
                                        _hasYelledMagmaBurst = true;
                                    }
                                }
                                events.ScheduleEvent(EVENT_MAGMA_BLAST, 2500);
                                break;
                            case EVENT_SUBMERGE:
                            {
                                if (instance && !_isBanished)
                                {
                                    //Creature spawning and ragnaros becomming unattackable
                                    //is not very well supported in the core //no it really isnt
                                    //so added normaly spawning and banish workaround and attack again after 90 secs.
                                    me->AttackStop();
                                    DoResetThreat();
                                    me->SetReactState(REACT_PASSIVE);
                                    me->InterruptNonMeleeSpells(false);
                                    //Root self
                                    //DoCast(me, 23973);
                                    me->setFaction(35);
                                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
                                    me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);
                                    instance->SetData(DATA_RAGNAROS_ADDS, 0);

                                    if (!_hasSubmergedOnce)
                                    {
                                        DoScriptText(SAY_REINFORCEMENTS1, me);

                                        // summon 8 elementals
                                        for (uint8 i = 0; i < 8; ++i)
                                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                                if (Creature* summoned = me->SummonCreature(12143, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 900000))
                                                    summoned->AI()->AttackStart(target);

                                        _hasSubmergedOnce = true;
                                        _isBanished = true;
                                        //DoCast(me, SPELL_RAGSUBMERGE);
                                        _emergeTimer = 90000;

                                    }
                                    else
                                    {
                                        DoScriptText(SAY_REINFORCEMENTS2, me);

                                        for (uint8 i = 0; i < 8; ++i)
                                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                                if (Creature* summoned = me->SummonCreature(12143, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 900000))
                                                    summoned->AI()->AttackStart(target);

                                        _isBanished = true;
                                        //DoCast(me, SPELL_RAGSUBMERGE);
                                        _emergeTimer = 90000;
                                    }
                                }
                                events.ScheduleEvent(EVENT_SUBMERGE, 180000);
                                break;
                            }
                            default:
                                break;
                        }
                    }

                    DoMeleeAttackIfReady();
                }
            }

        private:
            uint32 _emergeTimer;
            uint8 _introState;
            bool _hasYelledMagmaBurst;
            bool _hasSubmergedOnce;
            bool _isBanished;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_ragnarosAI(creature);
        }
};

class mob_son_of_flame : public CreatureScript
{
    public:
        mob_son_of_flame() : CreatureScript("mob_SonOfFlame") { }

        struct mob_son_of_flameAI : public ScriptedAI //didnt work correctly in EAI for me...
        {
            mob_son_of_flameAI(Creature* c) : ScriptedAI(c)
            {
                instance = me->GetInstanceScript();
            }

            void JustDied(Unit* /*victim*/)
            {
                if (instance)
                    instance->SetData(DATA_RAGNAROS_ADDS, 1);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_son_of_flameAI(creature);
        }
};

void AddSC_boss_ragnaros()
{
    new boss_ragnaros();
    new boss_ragnaros_outdoor();
    new mob_son_of_flame();
}
