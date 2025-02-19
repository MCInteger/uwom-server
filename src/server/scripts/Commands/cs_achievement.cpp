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
Name: achievement_commandscript
%Complete: 100
Comment: All achievement related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"

class achievement_commandscript : public CommandScript
{
public:
    achievement_commandscript() : CommandScript("achievement_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand achievementCommandTable[] =
        {
            { "add",        SEC_ADMINISTRATOR,  false,  &HandleAchievementAddCommand,           "", NULL },
            { "addconsole", SEC_CONSOLE,        true,   &HandleAchievementAddConsoleCommand,    "", NULL },
            { NULL,         0,                  false,  NULL,                                   "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "achievement",    SEC_ADMINISTRATOR,  false, NULL,    "", achievementCommandTable },
            { NULL,             0,                  false, NULL,    "", NULL }
        };

        return commandTable;
    }

    static bool HandleAchievementAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 achievementId = atoi((char*)args);
        if (!achievementId)
        {
            if (char* id = handler->extractKeyFromLink((char*)args, "Hachievement"))
                achievementId = atoi(id);
            if (!achievementId)
                return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (AchievementEntry const* achievementEntry = GetAchievementStore()->LookupEntry(achievementId))
            target->CompletedAchievement(achievementEntry);

        return true;
    }

    static bool HandleAchievementAddConsoleCommand(ChatHandler * /*handler*/, char const * args)
    {
        if (!*args)
            return false;

        Player * target = NULL;
        char * cname = NULL;
        char * cachieve = NULL;
        std::string name;
        uint32 achieveId = 0;

        cname = strtok((char*)args, " ");
        if (!cname)
            return false;
        else
        {
            name = cname;
            normalizePlayerName(name);
        }

        uint64 guid = sObjectMgr->GetPlayerGUIDByName(name);
        target = sObjectMgr->GetPlayerByLowGUID(GUID_LOPART(guid));
        if (!target || !target->IsInWorld())
            return false;

        cachieve = strtok(NULL, " ");
        if (!cachieve)
            return false;
        else
            achieveId = atol(cachieve);

        if (!achieveId)
                return false;

        if (AchievementEntry const * achievementEntry = GetAchievementStore()->LookupEntry(achieveId))
            target->CompletedAchievement(achievementEntry);
        else
            return false;

        return true;
    }
};

void AddSC_achievement_commandscript()
{
    new achievement_commandscript();
}
