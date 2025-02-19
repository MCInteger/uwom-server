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

#ifndef ICECROWN_CITADEL_H_
#define ICECROWN_CITADEL_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"

#define ICCScriptName "instance_icecrown_citadel"
#define Quest_A_Feast_of_Souls  24547

uint32 const EncounterCount = 13;
uint32 const WeeklyNPCs = 9;
uint32 const MaxHeroicAttempts = 50;

// Defined in boss_valithria_dreamwalker.cpp
extern Position const ValithriaSpawnPos;
// Defined in boss_sindragosa.cpp
extern Position const SindragosaSpawnPos;
// Defined in boss_the_lich_king.cpp
extern Position const TerenasSpawn;
extern Position const TerenasSpawnHeroic;
extern Position const SpiritWardenSpawn;

// Shared spells used by more than one script
enum SharedSpells
{
    SPELL_BERSERK                       = 26662,
    SPELL_BERSERK2                      = 47008,

    // Deathbound Ward
    SPELL_STONEFORM                     = 70733,

    // Residue Rendezvous
    SPELL_ORANGE_BLIGHT_RESIDUE         = 72144,
    SPELL_GREEN_BLIGHT_RESIDUE          = 72145,

    // Kill Credit 38121 - Soul Feast Kill Credit Bunny - Quest 24547 - A Feast of Souls
    // Must be used by every killable NPC within the ICC
    // The Lich King
    SPELL_FROSTMOURNE_TELEPORT_VISUAL   = 73078,
    SPELL_SOUL_FEAST_1                  = 71203,
    SPELL_SOUL_FEAST_2                  = 53677
};

enum TeleporterSpells
{
    DALARAN_TELEPORT                = 71512,
    LIGHT_S_HAMMER_TELEPORT         = 70781,
    ORATORY_OF_THE_DAMNED_TELEPORT  = 70856,
    RAMPART_OF_SKULLS_TELEPORT      = 70857,
    DEATHBRINGER_S_RISE_TELEPORT    = 70858,
    UPPER_SPIRE_TELEPORT            = 70859,
    FROZEN_THRONE_TELEPORT          = 70860,
    SINDRAGOSA_S_LAIR_TELEPORT      = 70861
};

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_LORD_MARROWGAR,
    DATA_LADY_DEATHWHISPER,
    DATA_GUNSHIP_EVENT,
    DATA_DEATHBRINGER_SAURFANG,
    DATA_FESTERGUT,
    DATA_ROTFACE,
    DATA_PROFESSOR_PUTRICIDE,
    DATA_BLOOD_PRINCE_COUNCIL,
    DATA_BLOOD_QUEEN_LANA_THEL,
    DATA_SISTER_SVALNA,
    DATA_VALITHRIA_DREAMWALKER,
    DATA_SINDRAGOSA,
    DATA_THE_LICH_KING,

    // Additional data
    DATA_SAURFANG_EVENT_NPC,
    DATA_BONED_ACHIEVEMENT,
    DATA_OOZE_DANCE_ACHIEVEMENT,
    DATA_PUTRICIDE_TABLE,
    DATA_NAUSEA_ACHIEVEMENT,
    DATA_ORB_WHISPERER_ACHIEVEMENT,
    DATA_PRINCE_KELESETH_GUID,
    DATA_PRINCE_TALDARAM_GUID,
    DATA_PRINCE_VALANAR_GUID,
    DATA_BLOOD_PRINCES_CONTROL,
    DATA_SINDRAGOSA_FROSTWYRMS,
    DATA_SPINESTALKER,
    DATA_RIMEFANG,
    DATA_COLDFLAME_JETS,
    DATA_TEAM_IN_INSTANCE,
    DATA_BLOOD_QUICKENING_STATE,
    DATA_HEROIC_ATTEMPTS,
    DATA_CROK_SCOURGEBANE,
    DATA_CAPTAIN_ARNATH,
    DATA_CAPTAIN_BRANDON,
    DATA_CAPTAIN_GRONDEL,
    DATA_CAPTAIN_RUPERT,
    DATA_VALITHRIA_TRIGGER,
    DATA_VALITHRIA_LICH_KING,
    DATA_HIGHLORD_TIRION_FORDRING,
    DATA_ARTHAS_PLATFORM,
    DATA_TERENAS_MENETHIL,

    DATA_FALLEN_ADDS_SPAWNEN,
    DATA_KILL_CREDIT
};

enum CreaturesIds
{
    // At Light's Hammer
    NPC_HIGHLORD_TIRION_FORDRING_LH             = 37119,
    NPC_THE_LICH_KING_LH                        = 37181,
    NPC_HIGHLORD_BOLVAR_FORDRAGON_LH            = 37183,
    NPC_KOR_KRON_GENERAL                        = 37189,
    NPC_ALLIANCE_COMMANDER                      = 37190,
    NPC_TORTUNOK                                = 37992,    // Druid Armor H
    NPC_ALANA_MOONSTRIKE                        = 37999,    // Druid Armor A
    NPC_GERARDO_THE_SUAVE                       = 37993,    // Hunter Armor H
    NPC_TALAN_MOONSTRIKE                        = 37998,    // Hunter Armor A
    NPC_UVLUS_BANEFIRE                          = 38284,    // Mage Armor H
    NPC_MALFUS_GRIMFROST                        = 38283,    // Mage Armor A
    NPC_IKFIRUS_THE_VILE                        = 37991,    // Rogue Armor H
    NPC_YILI                                    = 37997,    // Rogue Armor A
    NPC_VOL_GUK                                 = 38841,    // Shaman Armor H
    NPC_JEDEBIA                                 = 38840,    // Shaman Armor A
    NPC_HARAGG_THE_UNSEEN                       = 38181,    // Warlock Armor H
    NPC_NIBY_THE_ALMIGHTY                       = 38182,    // Warlock Armor N
    NPC_GARROSH_HELLSCREAM                      = 39372,
    NPC_KING_VARIAN_WRYNN                       = 39371,
    NPC_DEATHBOUND_WARD                         = 37007,

    // Weekly quests
    NPC_INFILTRATOR_MINCHAR                     = 38471,
    NPC_KOR_KRON_LIEUTENANT                     = 38491,
    NPC_SKYBREAKER_LIEUTENANT                   = 38492,
    NPC_ROTTING_FROST_GIANT_10                  = 38490,
    NPC_ROTTING_FROST_GIANT_25                  = 38494,
    NPC_ALCHEMIST_ADRIANNA                      = 38501,
    NPC_ALRIN_THE_AGILE                         = 38551,
    NPC_INFILTRATOR_MINCHAR_BQ                  = 38558,
    NPC_MINCHAR_BEAM_STALKER                    = 38557,
    NPC_VALITHRIA_DREAMWALKER_QUEST             = 38589,

    // Lord Marrowgar
    NPC_LORD_MARROWGAR                          = 36612,
    NPC_COLDFLAME                               = 36672,
    NPC_BONE_SPIKE                              = 36619,

    // Lady Deathwhisper
    NPC_LADY_DEATHWHISPER                       = 36855,
    NPC_CULT_FANATIC                            = 37890,
    NPC_DEFORMED_FANATIC                        = 38135,
    NPC_REANIMATED_FANATIC                      = 38009,
    NPC_CULT_ADHERENT                           = 37949,
    NPC_EMPOWERED_ADHERENT                      = 38136,
    NPC_REANIMATED_ADHERENT                     = 38010,
    NPC_VENGEFUL_SHADE                          = 38222,

    // Deathbringer Saurfang
    NPC_DEATHBRINGER_SAURFANG                   = 37813,
    NPC_BLOOD_BEAST                             = 38508,
    NPC_SE_JAINA_PROUDMOORE                     = 37188,    // SE means Saurfang Event
    NPC_SE_MURADIN_BRONZEBEARD                  = 37200,
    NPC_SE_KING_VARIAN_WRYNN                    = 37879,
    NPC_SE_HIGH_OVERLORD_SAURFANG               = 37187,
    NPC_SE_KOR_KRON_REAVER                      = 37920,
    NPC_SE_SKYBREAKER_MARINE                    = 37830,
    NPC_FROST_FREEZE_TRAP                       = 37744,

    // Festergut
    NPC_FESTERGUT                               = 36626,
    NPC_GAS_DUMMY                               = 36659,

    // Rotface
    NPC_ROTFACE                                 = 36627,
    NPC_OOZE_SPRAY_STALKER                      = 37986,
    NPC_PUDDLE_STALKER                          = 37013,
    NPC_UNSTABLE_EXPLOSION_STALKER              = 38107,

    // Professor Putricide
    NPC_PROFESSOR_PUTRICIDE                     = 36678,
    NPC_ABOMINATION_WING_MAD_SCIENTIST_STALKER  = 37824,
    NPC_GROWING_OOZE_PUDDLE                     = 37690,
    NPC_GAS_CLOUD                               = 37562,
    NPC_VOLATILE_OOZE                           = 37697,
    NPC_CHOKING_GAS_BOMB                        = 38159,
    NPC_TEAR_GAS_TARGET_STALKER                 = 38317,
    NPC_MUTATED_ABOMINATION_10                  = 37672,
    NPC_MUTATED_ABOMINATION_25                  = 38285,

    // Blood Prince Council
    NPC_PRINCE_KELESETH                         = 37972,
    NPC_PRINCE_TALDARAM                         = 37973,
    NPC_PRINCE_VALANAR                          = 37970,
    NPC_BLOOD_ORB_CONTROLLER                    = 38008,
    NPC_FLOATING_TRIGGER                        = 30298,
    NPC_DARK_NUCLEUS                            = 38369,
    NPC_BALL_OF_FLAME                           = 38332,
    NPC_BALL_OF_INFERNO_FLAME                   = 38451,
    NPC_KINETIC_BOMB_TARGET                     = 38458,
    NPC_KINETIC_BOMB                            = 38454,
    NPC_SHOCK_VORTEX                            = 38422,

    // Blood-Queen Lana'thel
    NPC_BLOOD_QUEEN_LANA_THEL                   = 37955,

    // Frostwing Halls gauntlet event
    NPC_CROK_SCOURGEBANE                        = 37129,
    NPC_CAPTAIN_ARNATH                          = 37122,
    NPC_CAPTAIN_BRANDON                         = 37123,
    NPC_CAPTAIN_GRONDEL                         = 37124,
    NPC_CAPTAIN_RUPERT                          = 37125,
    NPC_CAPTAIN_ARNATH_UNDEAD                   = 37491,
    NPC_CAPTAIN_BRANDON_UNDEAD                  = 37493,
    NPC_CAPTAIN_GRONDEL_UNDEAD                  = 37494,
    NPC_CAPTAIN_RUPERT_UNDEAD                   = 37495,
    NPC_YMIRJAR_BATTLE_MAIDEN                   = 37132,
    NPC_YMIRJAR_DEATHBRINGER                    = 38125,
    NPC_YMIRJAR_FROSTBINDER                     = 37127,
    NPC_YMIRJAR_HUNTRESS                        = 37134,
    NPC_YMIRJAR_WARLORD                         = 37133,
    NPC_SISTER_SVALNA                           = 37126,
    NPC_IMPALING_SPEAR                          = 38248,

    // Valithria Dreamwalker
    NPC_VALITHRIA_DREAMWALKER                   = 36789,
    NPC_GREEN_DRAGON_COMBAT_TRIGGER             = 38752,
    NPC_RISEN_ARCHMAGE                          = 37868,
    NPC_BLAZING_SKELETON                        = 36791,
    NPC_SUPPRESSER                              = 37863,
    NPC_BLISTERING_ZOMBIE                       = 37934,
    NPC_GLUTTONOUS_ABOMINATION                  = 37886,
    NPC_MANA_VOID                               = 38068,
    NPC_COLUMN_OF_FROST                         = 37918,
    NPC_ROT_WORM                                = 37907,
    NPC_THE_LICH_KING_VALITHRIA                 = 16980,
    NPC_DREAM_PORTAL_PRE_EFFECT                 = 38186,
    NPC_NIGHTMARE_PORTAL_PRE_EFFECT             = 38429,
    NPC_DREAM_PORTAL                            = 37945,
    NPC_NIGHTMARE_PORTAL                        = 38430,
    NPC_WORLD_TRIGGER                           = 22515,

    // Sindragosa
    NPC_SINDRAGOSA                              = 36853,
    NPC_SPINESTALKER                            = 37534,
    NPC_RIMEFANG                                = 37533,
    NPC_FROSTWARDEN_HANDLER                     = 37531,
    NPC_FROSTWING_WHELP                         = 37532,
    NPC_ICY_BLAST                               = 38223,
    NPC_FROST_BOMB                              = 37186,
    NPC_ICE_TOMB                                = 36980,

    // The Lich King
    NPC_THE_LICH_KING                           = 36597,
    NPC_HIGHLORD_TIRION_FORDRING_LK             = 38995,
    NPC_TERENAS_MENETHIL_FROSTMOURNE            = 36823,
    NPC_SPIRIT_WARDEN                           = 36824,
    NPC_TERENAS_MENETHIL_FROSTMOURNE_H          = 39217,
    NPC_SHAMBLING_HORROR                        = 37698,
    NPC_DRUDGE_GHOUL                            = 37695,
    NPC_ICE_SPHERE                              = 36633,
    NPC_RAGING_SPIRIT                           = 36701,
    NPC_DEFILE                                  = 38757,
    NPC_VALKYR_SHADOWGUARD                      = 36609,
    NPC_VILE_SPIRIT                             = 37799,
    NPC_WICKED_SPIRIT                           = 39190,
    NPC_STRANGULATE_VEHICLE                     = 36598,
    NPC_WORLD_TRIGGER_INFINITE_AOI              = 36171,
    NPC_SPIRIT_BOMB                             = 39189,
    NPC_FROSTMOURNE_TRIGGER                     = 38584
};

enum GameObjectsIds
{
    // Lower Spire Trash
    GO_SPIRIT_ALARM_1                       = 201814, // unten
    GO_SPIRIT_ALARM_2                       = 201815, // unten
    GO_SPIRIT_ALARM_3                       = 201816, // unten
    GO_SPIRIT_ALARM_4                       = 201817, // unten

    GO_SPIRIT_ALARM_5                       = 201888, // oben
    GO_SPIRIT_ALARM_6                       = 201889, // oben

    // Lord Marrogar
    GO_DOODAD_ICECROWN_ICEWALL02            = 201910,
    GO_ICEWALL                              = 201911,
    GO_LORD_MARROWGAR_S_ENTRANCE            = 201857,

    // Lady Deathwhisper
    GO_ORATORY_OF_THE_DAMNED_ENTRANCE       = 201563,
    GO_LADY_DEATHWHISPER_ELEVATOR           = 202220,

    // Kanonenschiff
    GO_Truhe_des_Kanonenschiffs_Ally_10     = 201872,
    GO_Truhe_des_Kanonenschiffs_Ally_10H    = 201873,
    GO_Truhe_des_Kanonenschiffs_Ally_25     = 201874,
    GO_Truhe_des_Kanonenschiffs_Ally_25H    = 201875,
    GO_Truhe_des_Kanonenschiffs_Horde_10    = 202177,
    GO_Truhe_des_Kanonenschiffs_Horde_10H   = 202178,
    GO_Truhe_des_Kanonenschiffs_Horde_25    = 202179,
    GO_Truhe_des_Kanonenschiffs_Horde_25H   = 202180,

    // Deathbringer Saurfang
    GO_SAURFANG_S_DOOR                      = 201825,
    GO_DEATHBRINGER_S_CACHE_10N             = 202239,
    GO_DEATHBRINGER_S_CACHE_25N             = 202240,
    GO_DEATHBRINGER_S_CACHE_10H             = 202238,
    GO_DEATHBRINGER_S_CACHE_25H             = 202241,
    GO_SCOURGE_TRANSPORTER_SAURFANG         = 202244,

    // Professor Putricide
    GO_ORANGE_PLAGUE_MONSTER_ENTRANCE       = 201371,
    GO_GREEN_PLAGUE_MONSTER_ENTRANCE        = 201370,
    GO_SCIENTIST_AIRLOCK_DOOR_COLLISION     = 201612,
    GO_SCIENTIST_AIRLOCK_DOOR_ORANGE        = 201613,
    GO_SCIENTIST_AIRLOCK_DOOR_GREEN         = 201614,
    GO_DOODAD_ICECROWN_ORANGETUBES02        = 201617,
    GO_DOODAD_ICECROWN_GREENTUBES02         = 201618,
    GO_SCIENTIST_ENTRANCE                   = 201372,
    GO_DRINK_ME                             = 201584,
    GO_PLAGUE_SIGIL                         = 202182,

    GO_Truhe_des_Traumwandlers_10           = 201959,
    GO_Truhe_des_Traumwandlers_10H          = 202338,
    GO_Truhe_des_Traumwandlers_25           = 202339,
    GO_Truhe_des_Traumwandlers_25H          = 202340,

    // Blood Prince Council
    GO_CRIMSON_HALL_DOOR                    = 201376,
    GO_BLOOD_ELF_COUNCIL_DOOR               = 201378,
    GO_BLOOD_ELF_COUNCIL_DOOR_RIGHT         = 201377,

    // Blood-Queen Lana'thel
    GO_DOODAD_ICECROWN_BLOODPRINCE_DOOR_01  = 201746,
    GO_DOODAD_ICECROWN_GRATE_01             = 201755,
    GO_BLOODWING_SIGIL                      = 202183,

    // Valithria Dreamwalker
    GO_GREEN_DRAGON_BOSS_ENTRANCE           = 201375,
    GO_GREEN_DRAGON_BOSS_EXIT               = 201374,
    GO_DOODAD_ICECROWN_ROOSTPORTCULLIS_01   = 201380,
    GO_DOODAD_ICECROWN_ROOSTPORTCULLIS_02   = 201381,
    GO_DOODAD_ICECROWN_ROOSTPORTCULLIS_03   = 201382,
    GO_DOODAD_ICECROWN_ROOSTPORTCULLIS_04   = 201383,

    // Sindragosa
    GO_SINDRAGOSA_ENTRANCE_DOOR             = 201373,
    GO_SINDRAGOSA_SHORTCUT_ENTRANCE_DOOR    = 201369,
    GO_SINDRAGOSA_SHORTCUT_EXIT_DOOR        = 201379,
    GO_ICE_WALL                             = 202396,
    GO_ICE_BLOCK                            = 201722,
    GO_SIGIL_OF_THE_FROSTWING               = 202181,

    // The Lich King
    GO_ARTHAS_PLATFORM                      = 202161,
    GO_ARTHAS_PRECIPICE                     = 202078,
    GO_DOODAD_ICECROWN_THRONEFROSTYWIND01   = 202188,
    GO_DOODAD_ICECROWN_THRONEFROSTYEDGE01   = 202189,
    GO_DOODAD_ICESHARD_STANDING02           = 202141,
    GO_DOODAD_ICESHARD_STANDING01           = 202142,
    GO_DOODAD_ICESHARD_STANDING03           = 202143,
    GO_DOODAD_ICESHARD_STANDING04           = 202144,
    GO_DOODAD_ICECROWN_SNOWEDGEWARNING01    = 202190,
    GO_FROZEN_LAVAMAN                       = 202436,
    GO_LAVAMAN_PILLARS_CHAINED              = 202437,
    GO_LAVAMAN_PILLARS_UNCHAINED            = 202438
};

enum AchievementCriteriaIds
{
    // Lord Marrowgar
    CRITERIA_BONED_10N                  = 12775,
    CRITERIA_BONED_25N                  = 12962,
    CRITERIA_BONED_10H                  = 13393,
    CRITERIA_BONED_25H                  = 13394,

    // Rotface
    CRITERIA_DANCES_WITH_OOZES_10N      = 12984,
    CRITERIA_DANCES_WITH_OOZES_25N      = 12966,
    CRITERIA_DANCES_WITH_OOZES_10H      = 12985,
    CRITERIA_DANCES_WITH_OOZES_25H      = 12983,

    // Professor Putricide
    CRITERIA_NAUSEA_10N                 = 12987,
    CRITERIA_NAUSEA_25N                 = 12968,
    CRITERIA_NAUSEA_10H                 = 12988,
    CRITERIA_NAUSEA_25H                 = 12981,

    // Blood Prince Council
    CRITERIA_ORB_WHISPERER_10N          = 13033,
    CRITERIA_ORB_WHISPERER_25N          = 12969,
    CRITERIA_ORB_WHISPERER_10H          = 13034,
    CRITERIA_ORB_WHISPERER_25H          = 13032,

    // Blood-Queen Lana'thel
    CRITERIA_KILL_LANA_THEL_10M         = 13340,
    CRITERIA_KILL_LANA_THEL_25M         = 13360,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_10N  = 12780,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_25N  = 13012,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_10V  = 13011,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_25V  = 13013
};

enum SharedActions
{
    // Festergut
    ACTION_FESTERGUT_COMBAT     = -366260,
    ACTION_FESTERGUT_GAS        = -366261,
    ACTION_FESTERGUT_DEATH      = -366262,

    // Rotface
    ACTION_ROTFACE_COMBAT       = -366270,
    ACTION_ROTFACE_OOZE         = -366271,
    ACTION_ROTFACE_DEATH        = -366272,
    ACTION_CHANGE_PHASE         = -366780,

    // Blood-Queen Lana'thel
    ACTION_KILL_MINCHAR         = -379550,

    // Frostwing Halls gauntlet event
    ACTION_VRYKUL_DEATH         = 37129,

    // Sindragosa
    ACTION_START_FROSTWYRM      = -368530,
    ACTION_TRIGGER_ASPHYXIATION = -368531,

    // The Lich King
    ACTION_RESTORE_LIGHT        = -72262,
    ACTION_FROSTMOURNE_INTRO    = -36823
};

enum WeekliesICC
{
    QUEST_DEPROGRAMMING_10                  = 24869,
    QUEST_DEPROGRAMMING_25                  = 24875,
    QUEST_SECURING_THE_RAMPARTS_10          = 24870,
    QUEST_SECURING_THE_RAMPARTS_25          = 24877,
    QUEST_RESIDUE_RENDEZVOUS_10             = 24873,
    QUEST_RESIDUE_RENDEZVOUS_25             = 24878,
    QUEST_BLOOD_QUICKENING_10               = 24874,
    QUEST_BLOOD_QUICKENING_25               = 24879,
    QUEST_RESPITE_FOR_A_TORNMENTED_SOUL_10  = 24872,
    QUEST_RESPITE_FOR_A_TORNMENTED_SOUL_25  = 24880
};

enum WorldStatesICC
{
    WORLDSTATE_SHOW_TIMER           = 4903,
    WORLDSTATE_EXECUTION_TIME       = 4904,
    WORLDSTATE_SHOW_ATTEMPTS        = 4940,
    WORLDSTATE_ATTEMPTS_REMAINING   = 4941,
    WORLDSTATE_ATTEMPTS_MAX         = 4942
};

enum AreaIds
{
    AREA_THE_FROZEN_THRONE  = 4859
};

class spell_trigger_spell_from_caster : public SpellScriptLoader
{
    public:
        spell_trigger_spell_from_caster(char const* scriptName, uint32 triggerId) : SpellScriptLoader(scriptName), _triggerId(triggerId) { }

        class spell_trigger_spell_from_caster_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_trigger_spell_from_caster_SpellScript);

        public:
            spell_trigger_spell_from_caster_SpellScript(uint32 triggerId) : SpellScript(), _triggerId(triggerId) { }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(_triggerId))
                    return false;
                return true;
            }

            void HandleTrigger()
            {
                GetCaster()->CastSpell(GetHitUnit(), _triggerId, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_trigger_spell_from_caster_SpellScript::HandleTrigger);
            }

            uint32 _triggerId;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_trigger_spell_from_caster_SpellScript(_triggerId);
        }

    private:
        uint32 _triggerId;
};

template<class AI>
CreatureAI* GetIcecrownCitadelAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(ICCScriptName))
                return new AI(creature);
    return NULL;
}

#endif // ICECROWN_CITADEL_H_
