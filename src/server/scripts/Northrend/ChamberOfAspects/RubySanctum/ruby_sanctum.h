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

#ifndef RUBY_SANCTUM_H_
#define RUBY_SANCTUM_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"

#define RSScriptName "instance_ruby_sanctum"

const uint32 EncounterCount = 5;
const Position HalionSpawnPos = { 3156.67f,  533.8108f, 72.98822f, 3.159046f };

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_BALTHARUS_THE_WARBORN,
    DATA_GENERAL_ZARITHRIAN,
    DATA_SAVIANA_RAGEFIRE,
    DATA_HALION,
    DATA_HALION_TWILIGHT,
    // Etc
    DATA_XERESTRASZA,
    DATA_XERESTRASZA_ALLOWED,
    DATA_CRYSTAL_CHANNEL_TARGET,
    DATA_ZARITHIAN_SPAWN_STALKER_1,
    DATA_ZARITHIAN_SPAWN_STALKER_2,
    DATA_HALION_CONTROLLER,
    DATA_HALION_CONTROLLER_SPAWNED,
    DATA_HALION_HEALTH,
    DATA_BURNING_TREE_1,
    DATA_BURNING_TREE_2,
    DATA_BURNING_TREE_3,
    DATA_BURNING_TREE_4,
    DATA_FLAME_RING,
    DATA_ZWIELICHT_RING,
    DATA_COUNTER,
    DATA_PHASE,
    // Schattenkugeln
    DATA_KUGELROTATIONSFOKUS,
    DATA_SCHATTENKUGEL_N        = 100,
    DATA_SCHATTENKUGEL_S        = 101,
    DATA_SCHATTENKUGEL_O        = 102,
    DATA_SCHATTENKUGEL_W        = 103,
    DATA_KUGEL_RICHTUNG         = 104
};

enum SharedActions
{
    ACTION_INTRO_BALTHARUS  = -3975101,
    ACTION_BALTHARUS_DEATH  = -3975102,
    ACTION_INTRO_HALION     = -4014601,
    ACTION_SPAWN_HALION     = -4014602
};

enum CreaturesIds
{
    // Baltharus the Warborn
    NPC_BALTHARUS_THE_WARBORN       = 39751,
    NPC_BALTHARUS_THE_WARBORN_CLONE = 39899,
    NPC_BALTHARUS_TARGET            = 26712,
    // General Zarithrian
    NPC_GENERAL_ZARITHRIAN          = 39746,
    NPC_ONYX_FLAMECALLER            = 39814,
    NPC_ZARITHIAN_SPAWN_STALKER     = 39794,
    // Saviana Ragefire
    NPC_SAVIANA_RAGEFIRE            = 39747,
    // Halion
    NPC_HALION                      = 39863,
    NPC_HALION_TWILIGHT             = 40142,
    NPC_HALION_CONTROLLER           = 40146,
    NPC_LIVING_INFERNO              = 40681,
    NPC_LIVING_EMBER                = 40683,
    NPC_METEOR_STRIKE_MARK          = 40029,
    NPC_METEOR_STRIKE_NORTH         = 40041,
    NPC_METEOR_STRIKE_EAST          = 40042,
    NPC_METEOR_STRIKE_WEST          = 40043,
    NPC_METEOR_STRIKE_SOUTH         = 40044,
    NPC_METEOR_STRIKE_FLAME         = 40055,
    NPC_COMBUSTION                  = 40001,
    // Schnitter
    NPC_KUGELTRAEGER                = 40081,
    NPC_KUGELROTATIONSFOKUS         = 40091,
    NPC_SCHATTENKUGEL_N             = 40083,
    NPC_SCHATTENKUGEL_S             = 40100,
    NPC_SCHATTENKUGEL_O             = 40469,
    NPC_SCHATTENKUGEL_W             = 40468,
    // Xerestrasza
    NPC_XERESTRASZA                 = 40429
};

enum GameObjectsIds
{
    GO_HALION_PORTAL_1      = 202794,   // Unknown spell 75074, should be somehow be linked to 74807
    GO_HALION_PORTAL_2      = 202795,   // Also spell 75074
    GO_HALION_PORTAL_EXIT   = 202796,   // Leave Twilight Realm (74812)
    GO_FIRE_FIELD           = 203005,
    GO_FLAME_WALLS          = 203006,
    GO_FLAME_RING           = 203007,
    GO_ZWIELICHT_RING       = 203624,
    GO_BURNING_TREE_1       = 203034,
    GO_BURNING_TREE_2       = 203035,
    GO_BURNING_TREE_3       = 203036,
    GO_BURNING_TREE_4       = 203037
};

enum WorldStatesRS
{
    WORLDSTATE_CORPOREALITY_MATERIAL = 5049,
    WORLDSTATE_CORPOREALITY_TWILIGHT = 5050,
    WORLDSTATE_CORPOREALITY_TOGGLE   = 5051
};

template<class AI>
CreatureAI * GetRubySanctumAI(Creature * creature)
{
    if (InstanceMap * instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(RSScriptName))
                return new AI(creature);
    return NULL;
}

#endif // RUBY_SANCTUM_H_
