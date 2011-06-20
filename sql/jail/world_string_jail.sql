﻿SET NAMES utf8;

DELETE FROM `trinity_string` WHERE `entry` BETWEEN 12100 AND 12199;
INSERT INTO `trinity_string` (`entry`,`content_default`,`content_loc3`) VALUES
(12100, 'You are jailed by %s for %u hour(s)!', 'Du wurdest von %s für %u Stunde(n) eingebuchtet!'),
(12101, '%s wrote this as reason: %s', '%s gab dies als Grund an: %s'),
(12102, '%s was jailed by you for %u hour(s).', '%s wurde von dir für %u Stunde(n) eingebuchtet.'),
(12103, 'You was released out of the jail by %s.', '%s hat dich aus dem Knast entlassen.'),
(12104, 'You have released %s out of the jail.', 'Du hast %s aus dem Knast geholt.'),
(12105, 'No reason given or reason is < %u chars!', 'Du hast keinen Grund angegeben, oder der Grund ist < %u Zeichen!'),
(12106, 'No name given!', 'Du hast keinen Namen angegeben!'),
(12107, 'No time given!', 'Du hast keine Zeit angegeben!'),
(12108, 'The jailtime must be between 1 and %u hours!', 'Die Jailzeit muss zwischen 1 und %u Std. liegen!'),
(12109, 'The character %s is not jailed!', 'Der Charakter %s ist überhaupt nicht im Knast!'),
(12110, 'Command forbidden for jailed characters!', 'Sorry, aber das dürfen Gefangene nicht!'),
(12111, 'You have %u hour(s) and %u minute(s) left in the jail.', 'Du musst noch %u Stunde(n) und %u Minute(n) absitzen.'),
(12112, 'You have %u minute(s) left in the jail.', 'Du musst noch %u Minute(n) absitzen.'),
(12113, 'You are free like a bird! ;-)', 'Du bist frei, wie ein Vogel! ;-)'),
(12114, '%s was %u times jailed and has %u minute(s) left.\nLast time jailed by: %s.\nThe last jail was on: %s\nThe last reason was: %s', '%s war bis jetzt %u mal im Knast, und hat noch %u Minute(n) abzusitzen.\nZuletzt eingebuchtet von: %s\nZuletzt eingebuchtet am: %s\nDer letzte Grund war: %s'),
(12115, '%s was never jailed.', '%s hat eine weisse Weste.'),
(12116, 'You can not jail yourself!', 'Du kannst dich nicht selber einbuchten!'),
(12117, 'You can not unjail yourself!', 'So weit kommt es noch, dass Knastbrüder sich selber befreien! :-('),
(12118, 'Be carefull!\nOnly one more jailtime and your current character will be deleted!', '!!! ACHTUNG - WICHTIG - WARNUNG !!!\nBeim nächsten Knastbesuch wird dieser Charakter gelöscht!'),
(12119, 'The character %s was jailed for %u hour(s) by the GM character %s!\nThe reason is: %s', 'Der Charakter %s wurde für %u Stunde(n) vom GM-Charakter %s eingebuchtet!\nDer Grund ist: %s'),
(12120, 'Jail: The configuration was reloaded.', 'Jail: Die Konfiguration wurde neu geladen.'),
(12121, 'Jail: Configuration loaded.', 'Jail: Konfiguration geladen.'),
(12122, 'Jail: Can not load the configuration! The table is empty or missed! Use the character_jail.sql!', 'Jail: Fehler beim laden der Konfiguration! Die Tabelle ist leer, oder nicht vorhanden! Nutze die character_jail.sql!'),
(12123, 'Jail: Set all config settings to default values.', 'Jail: Setze die Konfiguration auf Standardwerte.'),
(12124, 'The Character %s (GUID %u) is a jail brother and teleported into the jail.', 'Der Charakter %s (GUID %u) ist ein Knastbruder, und wird in den Knast teleportiert.'),
(12125, 'The Character %s (GUID %u) was released out of the jail.', 'Der Charakter %s (GUID %u) wurde aus dem Knast entlassen.'),
(12126, 'A character with this name does not exists!', 'Ein Charakter mit diesem Namen gibt es nicht!'),
(12127, 'Be carefull!\nOnly one more jailtime and your account will be banned!', '!!! ACHTUNG - WICHTIG - WARNUNG !!!\nBeim nächsten Knastbesuch wird dein Account gebannt!'),
(12128, 'Max. jailtimes reached!', 'Maximale Anzahl an Jails erreicht!'),
-- (12129, 'Robotron', 'Robotron'),
(12130, '%s was %u times jailed and has %u hour(s) and %u minute(s) left.\nLast time jailed by: %s.\nThe last jail was on: %s\nThe last reason was: %s', '%s war bis jetzt %u mal im Knast, und hat noch %u Stunde(n) und %u Minuten abzusitzen.\nZuletzt eingebuchtet von: %s\nZuletzt eingebuchtet am: %s\nDer letzte Grund war: %s'),
(12131, '%s was multiple times jailed and has already %u bans!', '%s wurde bereits mehrfach eingebuchtet, und hat bereits %u Bannung(en)!'),
(12132, 'The account of this character has further %u bans!','Der Account von ihm wurde ausserdem bereits %u mal gebannt!'),
(12133, '%s is already in the Jail!','%s ist bereits im Knast!'),
(12134, 'You can not reset a Jail entry with a banned account!','Das Löschen eines Jaileintrags, dessen Account gebannt wurde, ist nicht möglich!'),
(12197, 'The Jail is now active.', 'Das Jail ist nun aktiv.'),
(12198, 'The Jail is now inactive.', 'Das Jail ist nun deaktiviert.'),
(12199, 'The Jail is not active!', 'Das Jail ist nicht aktiv!');
