-- Replace Wintergrasp spawns with properly phased spawns
SET @GUID := 290000;
DELETE FROM `creature` WHERE `id` IN (31841,31842,30400,30499,30489,30869,31036,31051,31052,31054,31108,31109,31153,39172,30870,31053,31091,31101,31102,31106,31107,31151,32294,39173,32296,30488);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`) VALUES
-- Taunka Spirit Guide (Horde)
(@GUID+0,31841,571,1,0x00000010,0,0,4318.436,2408.06738,392.675751,6.23082542,120,0,0,1,0,0), -- Eastspark Workshop
(@GUID+1,31841,571,1,0x00000010,0,0,4336.25439,3235.51978,390.334,0.628318548,120,0,0,1,0,0), -- Westspark Workshop
(@GUID+2,31841,571,1,0x00000010,0,0,5104.753,2300.9458,368.568054,0.7330383,120,0,0,1,0,0), -- The Sunken Ring
(@GUID+3,31841,571,1,0x00000010,0,0,5103.133,3462.128,368.568054,5.270895,120,0,0,1,0,0), -- The Chilled Quagmire "Outside broker Temple"
(@GUID+4,31841,571,1,0x00000040,0,0,5537.482,2898.90674,517.2589,4.86946869,120,0,0,1,0,0), -- Fortress
(@GUID+5,31841,571,1,0x00000001,0,0,5031.84131,3710.74878,372.4835,0,120,0,0,1,0,0), -- Horde Landing Zone
-- Dwarven Spirit Guide (Alliance)
(@GUID+6,31842,571,1,0x00000020,0,0,4318.436,2408.06738,392.675751,6.23082542,120,0,0,1,0,0), -- Eastspark Workshop
(@GUID+7,31842,571,1,0x00000020,0,0,4336.25439,3235.51978,390.334,0.628318548,120,0,0,1,0,0), -- Westspark Workshop
(@GUID+8,31842,571,1,0x00000020,0,0,5104.753,2300.9458,368.568054,0.7330383,120,0,0,1,0,0), -- The Sunken Ring
(@GUID+9,31842,571,1,0x00000020,0,0,5103.133,3462.128,368.568054,5.270895,120,0,0,1,0,0), -- The Chilled Quagmire "Outside The Broken Temple"
(@GUID+10,31842,571,1,0x00000080,0,0,5537.482,2898.90674,517.2589,4.86946869,120,0,0,1,0,0), -- Fortress
(@GUID+11,31842,571,1,0x00000001,0,0,5140.7876,2179.12451,390.9512,1.97222209,120,0,0,1,0,0), -- Aliance Landing Zone
-- Goblin Mechanic <Demolisher Engineer> (Horde)
(@GUID+12,30400,571,1,0x00000010,0,0,4964.89,3383.06,382.911,6.12611,180,0,0,1,0,0), -- The Broken Temple
(@GUID+13,30400,571,1,0x00000010,0,0,4939.76,2389.06,326.153,3.26377,180,0,0,1,0,0), -- The Sunken Ring
(@GUID+14,30400,571,1,0x00000010,0,0,4357.67,2357.99,382.007,1.67552,180,0,0,1,0,0), -- Eastspark Workshop
(@GUID+15,30400,571,1,0x00000010,0,0,4354.15,3312.82,378.046,1.67552,180,0,0,1,0,0), -- Westspark Workshop
(@GUID+16,30400,571,1,0x00000040,0,0,5391.61,2707.72,415.051,4.55531,180,0,0,1,0,0), -- Wintergrasp Fortress
(@GUID+17,30400,571,1,0x00000040,0,0,5392.91,2975.26,415.223,4.55531,180,0,0,1,0,0), -- Wintergrasp Fortress
-- Gnomish Engineer <Demolisher Engineer> (Alliance)
(@GUID+18,30499,571,1,0x00000020,0,0,4964.89,3383.06,382.911,6.12611,180,0,0,1,0,0), -- The Broken Temple
(@GUID+19,30499,571,1,0x00000020,0,0,4939.76,2389.06,326.153,3.26377,180,0,0,1,0,0), -- The Sunken Ring
(@GUID+20,30499,571,1,0x00000020,0,0,4357.67,2357.99,382.007,1.67552,180,0,0,1,0,0), -- Eastspark Workshop
(@GUID+21,30499,571,1,0x00000020,0,0,4354.15,3312.82,378.046,1.67552,180,0,0,1,0,0), -- Westspark Workshop
(@GUID+22,30499,571,1,0x00000080,0,0,5391.61,2707.72,415.051,4.55531,180,0,0,1,0,0), -- Wintergrasp Fortress
(@GUID+23,30499,571,1,0x00000080,0,0,5392.91,2975.26,415.223,4.55531,180,0,0,1,0,0), -- Wintergrasp Fortress
-- Alliance NPC's
(@GUID+24,30489,571,1,0x00000080,0,0,5369.973,2874.83081,409.3225,3.12413931,120,0,0,1,0,0), -- Morgan Day <Jewelcrafter>
(@GUID+25,30869,571,1,0x00000001,0,0,5102.75049,2187.82837,365.707855,3.996804,120,0,0,1,0,0), -- Arzo Safeflight <Flight Master>
(@GUID+26,31036,571,1,0x00000040,0,0,5078.281,2183.704,365.028564,1.46607661,120,0,0,1,0,0), -- Commander Zanneth
(@GUID+27,31036,571,1,0x00000080,0,0,5358.641,2841.76416,409.3225,1.13262534,120,0,0,1,0,0),
(@GUID+28,31051,571,1,0x00000040,0,0,5081.697,2173.73,365.8777,0.8552113,120,0,0,1,0,0), -- Sorceress Kaylana <Enchantress>
(@GUID+29,31051,571,1,0x00000080,0,0,5296.869,2887.67114,409.274658,5.60250664,120,0,0,1,0,0),
(@GUID+30,31052,571,1,0x00000040,0,0,5100.06543,2168.89,365.7788,1.97222209,120,0,0,1,0,0), -- Bowyer Randolph
(@GUID+31,31052,571,1,0x00000080,0,0,5302.57373,2750.40332,409.274658,5.46288061,120,0,0,1,0,0),
(@GUID+32,31054,571,1,0x00000040,0,0,5088.611,2167.66235,365.688751,0.6806784,120,0,0,1,0,0), -- Anchorite Tessa
(@GUID+33,31054,571,1,0x00000080,0,0,5372.05859,3028.33618,409.206024,0.012565271,120,0,0,1,0,0),
(@GUID+34,31108,571,1,0x00000040,0,0,5095.673,2193.284,365.9236,4.939282,120,0,0,1,0,0), -- Siege Master Stouthandle
(@GUID+35,31108,571,1,0x00000080,0,0,5298.267,2924.97632,409.274658,0.9075712,120,0,0,1,0,0),
(@GUID+36,31109,571,1,0x00000040,0,0,5080.403,2199.00244,359.4894,2.96705961,120,0,0,1,0,0), -- Senior Demolitionist Legoso
(@GUID+37,31109,571,1,0x00000080,0,0,5228.2915,2809.888,409.274658,3.19395256,120,0,0,1,0,0),
(@GUID+38,31153,571,1,0x00000040,0,0,5088.48633,2188.17871,365.646973,5.253441,120,0,0,1,0,0), -- Tactical Officer Ahbramis
(@GUID+39,31153,571,1,0x00000080,0,0,5364.784,2835.135,409.3225,3.12413931,120,0,0,1,0,0),
(@GUID+40,32294,571,1,0x00000080,0,0,5374.568,2790.784,409.3225,2.72271371,120,0,0,1,0,0), -- Knight Dameron <Wintergrasp Quartermaster>
(@GUID+41,39172,571,1,0x00000080,0,0,5372.672,2786.74048,409.4423,2.80998015,120,0,0,1,0,0), -- Marshal Magruder <Wintergrasp Quartermaster>
(@GUID+42,30488,571,1,0x00000080,0,0,5370.428,2814.274,409.3225,3.054326,120,0,0,1,0,0), -- Travis Day <Legacy Weaponsmith>
-- Horde NPC's
(@GUID+43,30870,571,1,0x00000001,0,0,5023.4043,3686.03345,363.1192,5.131268,120,0,0,1,0,0), -- Herzo Safeflight <Flight Master>
(@GUID+44,31053,571,1,0x00000040,0,0,5379.875,3027.43359,409.206024,0,120,0,0,1,0,0), -- Primalist Mulfort
(@GUID+45,31053,571,1,0x00000080,0,0,5034.703125,3666.703125,363.273865,4.310963,120,0,0,1,0,0),
(@GUID+46,31091,571,1,0x00000040,0,0,5347.7915,2837.38574,409.3466,2.62603331,120,0,0,1,0,0), -- Commander Dardosh
(@GUID+47,31091,571,1,0x00000080,0,0,5018.662109,3672.279541,362.862885,2.209141,120,0,0,1,0,0),
(@GUID+48,31101,571,1,0x00000040,0,0,5296.564,2789.87378,409.274658,0.7330383,120,0,0,1,0,0), -- Hoodoo Master Fu'jin <Master Hexxer>
(@GUID+49,31101,571,1,0x00000080,0,0,5014.065430,3678.846436,362.995575,5.096361,120,0,0,1,0,0),
(@GUID+50,31102,571,1,0x00000040,0,0,5295.455,2732.87549,409.274658,4.7211113,120,0,0,1,0,0), -- Vieron Blazefeather
(@GUID+51,31102,571,1,0x00000080,0,0,5031.676270,3655.820801,362.234558,5.131268,120,0,0,1,0,0),
(@GUID+52,31106,571,1,0x00000040,0,0,5295.56348,2926.67188,409.274658,0.87266463,120,0,0,1,0,0), -- Siegesmith Stronghoof
(@GUID+53,31106,571,1,0x00000080,0,0,5037.602051,3675.459717,363.147888,3.176499,120,0,0,1,0,0),
(@GUID+54,31107,571,1,0x00000040,0,0,5230.09033,2876.635,409.316254,-2.19854617,120,0,0,1,0,0), -- Lieutenant Murp
(@GUID+55,31107,571,1,0x00000080,0,0,5004.455078,3661.089111,361.335785,3.979351,120,0,0,1,0,0),
(@GUID+56,31151,571,1,0x00000040,0,0,5363.2876,2834.52954,409.3606,2.76273036,120,0,0,1,0,0), -- Tactical Officer Kilrath
(@GUID+57,31151,571,1,0x00000080,0,0,5034.698242,3683.268799,363.129120,4.310963,120,0,0,1,0,0),
(@GUID+58,32296,571,1,0x00000040,0,0,5374.568,2790.784,409.3225,2.60054,120,0,0,1,0,0), -- Stone Guard Mukar <Wintergrasp Quartermaster>
-- Missing noc in fortress
(@GUID+60,39173,571,1,0x00000040,0,0,5372.672,2786.74048,409.4423,2.80998015,120,0,0,1,0,0); -- Champion Ros'slai <Wintergrasp Quartermaster>

-- Pathing for Anchorite Tessa Entry: 31054
SET @NPC := @GUID+33;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`bytes2`,`mount`,`auras`) VALUES (@NPC,@PATH,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5382.507,3028.468,409.206,0,0,0,100,0),
(@PATH,2,5392.744,3028.737,409.206,0,0,0,100,0),
(@PATH,3,5382.507,3028.468,409.206,0,0,0,100,0),
(@PATH,4,5371.491,3028.329,409.206,0,0,0,100,0),
(@PATH,5,5360.032,3028.516,409.3161,0,0,0,100,0),
(@PATH,6,5371.491,3028.329,409.206,0,0,0,100,0);

-- Pathing for Commander Zanneth Entry: 31036
SET @NPC := @GUID+27;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=5358.854,`position_y`=2859.232,`position_z`=409.5425 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`bytes2`,`mount`,`auras`) VALUES (@NPC,@PATH,1,14337, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5359.788,2846.359,409.3642,0,0,0,100,0),
(@PATH,2,5359.187,2839.211,409.3642,0,0,0,100,0),
(@PATH,3,5360.083,2823.116,409.5381,0,0,0,100,0),
(@PATH,4,5359.187,2839.211,409.3642,0,0,0,100,0),
(@PATH,5,5359.788,2846.359,409.3642,0,0,0,100,0),
(@PATH,6,5358.854,2859.232,409.5425,0,0,0,100,0);

-- Pathing for Commander Dardosh Entry: 31091
SET @NPC := @GUID+46;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `MovementType`=2,`position_x`=5359.546,`position_y`=2858.049,`position_z`=409.3642 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`bytes2`,`mount`,`auras`) VALUES (@NPC,@PATH,257,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5359.202,2843.208,409.3642,0,0,0,100,0),
(@PATH,2,5358.997,2827.974,409.3639,0,0,0,100,0),
(@PATH,3,5359.202,2843.208,409.3642,0,0,0,100,0),
(@PATH,4,5359.546,2858.049,409.3642,0,0,0,100,0);

-- Pathing for Commander Dardosh Entry: 31091
SET @NPC := @GUID+47;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `MovementType`=2,`position_x`=5018.411133,`position_y`=3672.615967,`position_z`=362.863525 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`bytes2`,`mount`,`auras`) VALUES (@NPC,@PATH,257,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5022.827637,3666.665771,362.205750,0,0,0,100,0),
(@PATH,2,5024.270508,3658.739258,361.705750,0,0,0,100,0),
(@PATH,3,5022.827637,3666.665771,362.205750,0,0,0,100,0),
(@PATH,4,5018.411133,3672.615967,362.863525,0,0,0,100,0);

-- Pathing for Primalist Mulfort Entry: 31053
SET @NPC := @GUID+44;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`bytes2`,`mount`,`auras`) VALUES (@NPC,@PATH,257,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5390.624,3027.994,409.206,0,0,0,100,0),
(@PATH,2,5379.875,3027.434,409.206,0,0,0,100,0),
(@PATH,3,5371.404,3026.511,409.206,0,0,0,100,0),
(@PATH,4,5379.875,3027.434,409.206,0,0,0,100,0);

-- Pathing for Vieron Blazefeather Entry: 31102
SET @NPC := @GUID+50;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `MovementType`=2,`position_x`=5296.644,`position_y`=2731.107,`position_z`=409.3163 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`bytes2`,`mount`,`auras`) VALUES (@NPC,@PATH,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5295.544,2722.631,409.3163,0,0,0,100,0),
(@PATH,2,5296.644,2731.107,409.3163,0,0,0,100,0),
(@PATH,3,5297.874,2738.155,409.3163,0,0,0,100,0),
(@PATH,4,5305.192,2746.161,409.3061,0,0,0,100,0),
(@PATH,5,5297.874,2738.155,409.3163,0,0,0,100,0),
(@PATH,6,5296.644,2731.107,409.3163,0,0,0,100,0);

-- Guards "Alliance" (Valiance Expedition Champion)
SET @GUID := 291000;
DELETE FROM `creature` WHERE `id`=30740;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`) VALUES
-- Fortress
(@GUID+0,30740,571,1,0x00000080,0,0,5150.26855,2786.02173,409.5469,3.22885919,120,0,0,1,0,0),
(@GUID+1,30740,571,1,0x00000080,0,0,5150.657,2777.9502,409.4053,2.94960642,120,0,0,1,0,0),
(@GUID+2,30740,571,1,0x00000080,0,0,5193.632,2734.172,409.272552,4.694936,120,0,0,1,0,0),
(@GUID+3,30740,571,1,0x00000080,0,0,5200.497,2733.88843,409.272552,4.677482,120,0,0,1,0,0),
(@GUID+4,30740,571,1,0x00000080,0,0,5264.64648,2670.1936,409.1819,3.07177949,120,0,0,1,0,0),
(@GUID+5,30740,571,1,0x00000080,0,0,5265.30566,2663.15381,409.1819,3.12413931,120,0,0,1,0,0),
(@GUID+6,30740,571,1,0x00000080,0,0,5307.02051,2613.89771,409.172363,4.694936,120,0,0,1,0,0),
(@GUID+7,30740,571,1,0x00000080,0,0,5311.133,3061.04248,408.809937,1.50098312,120,0,0,1,0,0),
(@GUID+8,30740,571,1,0x00000080,0,0,5315.87354,2614.21924,408.972748,4.677482,120,0,0,1,0,0),
(@GUID+9,30740,571,1,0x00000080,0,0,5318.09,3060.634,408.882782,1.62315619,120,0,0,1,0,0),
(@GUID+10,30740,571,1,0x00000080,0,0,5149.395,2897.044,409.304443,3.03687286,120,0,0,1,0,0),
(@GUID+11,30740,571,1,0x00000080,0,0,5149.51953,2904.19336,409.276062,3.07177949,120,0,0,1,0,0),
(@GUID+12,30740,571,1,0x00000080,0,0,5192.79248,2948.454,409.2746,1.50098312,120,0,0,1,0,0),
(@GUID+13,30740,571,1,0x00000080,0,0,5201.645,2948.77612,409.2746,1.62315619,120,0,0,1,0,0),
(@GUID+14,30740,571,1,0x00000080,0,0,5265.28,3010.10083,408.895782,2.82743335,120,0,0,1,0,0),
(@GUID+15,30740,571,1,0x00000080,0,0,5265.47559,3017.39941,408.582977,3.07177949,120,0,0,1,0,0),
(@GUID+16,30740,571,1,0x00000080,0,0,5367.91455,2826.52026,409.3225,3.33357882,120,0,0,1,0,0),
(@GUID+17,30740,571,1,0x00000080,0,0,5368.71338,2856.36035,409.3225,2.94960642,120,0,0,1,0,0),
(@GUID+18,30740,571,1,0x00000080,0,0,5388.56,2834.76782,418.7585,3.07177949,120,0,0,1,0,0),
(@GUID+19,30740,571,1,0x00000080,0,0,5389.272,2847.36816,418.7585,3.106686,120,0,0,1,0,0),
(@GUID+20,30740,571,1,0x00000080,0,0,4684.475,2414.28979,369.9621,-2.85779858,120,0,0,1,0,0),
(@GUID+21,30740,571,1,0x00000080,0,0,4692.75635,2392.88574,369.0177,-2.82921553,120,0,0,1,0,0),
(@GUID+22,30740,571,1,0x00000080,0,0,5327.264648,2659.455322,409.178711,3.069901,120,0,0,1,0,0), -- F1307814000A2DAD path
(@GUID+23,30740,571,1,0x00000080,0,0,5154.225586,2833.824219,409.262451,3.124139,120,0,0,1,0,0),
(@GUID+24,30740,571,1,0x00000080,0,0,5154.119629,2847.892822,409.247559,3.071779,120,0,0,1,0,0),
(@GUID+25,30740,571,1,0x00000080,0,0,5179.111328,2837.129639,409.274658,3.211406,120,0,0,1,0,0),
(@GUID+26,30740,571,1,0x00000080,0,0,5179.666504,2846.597900,409.274658,3.089233,120,0,0,1,0,0),
(@GUID+27,30740,571,1,0x00000080,0,0,5270.163086,2833.479248,409.274658,3.124139,120,0,0,1,0,0),
(@GUID+28,30740,571,1,0x00000080,0,0,5270.057129,2847.547607,409.274658,3.071779,120,0,0,1,0,0),
(@GUID+29,30740,571,1,0x00000080,0,0,5307.750000,3008.872559,409.193024,4.781681,120,0,0,1,0,0), -- F1307814000A43AE path
(@GUID+30,30740,571,1,0x00000080,0,0,5335.117188,2916.802002,409.443756,1.500983,120,0,0,1,0,0),
(@GUID+31,30740,571,1,0x00000080,0,0,5350.681152,2917.011719,409.274658,1.466077,120,0,0,1,0,0),
(@GUID+32,30740,571,1,0x00000080,0,0,5335.306152,2764.110352,409.274567,4.834562,120,0,0,1,0,0),
(@GUID+33,30740,571,1,0x00000080,0,0,5349.811523,2763.634766,409.333374,4.660029,120,0,0,1,0,0),
-- Eastspark Workshop
(@GUID+34,30740,571,1,0x00000080,0,0,4349.537,2411.25781,374.743317,2.05948853,120,0,0,1,0,0),
(@GUID+35,30740,571,1,0x00000080,0,0,4388.13135,2411.97827,374.743317,1.6406095,120,0,0,1,0,0),
(@GUID+36,30740,571,1,0x00000080,0,0,4391.6665,2300.60913,374.7433,4.92182827,120,0,0,1,0,0),
(@GUID+37,30740,571,1,0x00000080,0,0,4413.42969,2393.44946,376.3599,1.06465089,120,0,0,1,0,0),
(@GUID+38,30740,571,1,0x00000080,0,0,4417.92,2331.237,370.9189,5.846853,120,0,0,1,0,0),
(@GUID+39,30740,571,1,0x00000080,0,0,4349.11768,2299.27954,374.7433,4.904375,120,0,0,1,0,0),
(@GUID+40,30740,571,1,0x00000080,0,0,4418.608,2355.28735,372.4907,6.02138567,120,0,0,1,0,0);

-- banners
SET @OGUID := 281000;
DELETE FROM `gameobject` WHERE `id` IN (192254,192255,192269,192284,192285,192286,192287,192292,192299,192304,192305,192306,192307,192308,192309,192310,192312,192313,192314,192316,192317,192318,192319,192320,192321,
                                        192322,192323,192324,192325,192326,192327,192328,192329,192330,192331,192332,192333,192334,192335,192336,192338,192339,192349,192350,192351,192352,192353,192354,192355,192356,
										192357,192358,192359,192360,192361,192362,192363,192364,192366,192367,192368,192369,192370,192371,192372,192373,192374,192375,192376,192377,192378,192379,192487,192488,192501,
										192502);
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID AND @OGUID+115;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
-- Fortress Banners "Alliance"
(@OGUID+0,192286,571,1,128,5371.44873,2820.79346,409.426575,3.124123,0,0,0,0,120,0,1),
(@OGUID+1,192287,571,1,128,5372.42432,2862.47925,409.3659,3.14159274,0,0,0,0,120,0,1),
(@OGUID+2,192292,571,1,128,5154.35059,2862.08423,445.0105,3.14159274,0,0,0,0,120,0,1),
(@OGUID+3,192299,571,1,128,5155.21631,2820.63013,444.9789,-3.115388,0,0,0,0,120,0,1),
(@OGUID+4,192304,571,1,128,5398.03564,2873.013,455.203552,3.132858,0,0,0,0,120,0,1),
(@OGUID+5,192305,571,1,128,5397.31348,2809.264,455.101624,3.132858,0,0,0,0,120,0,1),
(@OGUID+6,192306,571,1,128,5270.55469,2861.68237,444.917236,-3.124123,0,0,0,0,120,0,1),
(@OGUID+7,192307,571,1,128,5271.16064,2820.10864,445.109,-3.132858,0,0,0,0,120,0,1),
(@OGUID+8,192308,571,1,128,5235.12744,2942.12329,444.2792,1.58824873,0,0,0,0,120,0,1),
(@OGUID+9,192309,571,1,128,5272.549,2976.54175,444.493,3.132858,0,0,0,0,120,0,1),
(@OGUID+10,192310,571,1,128,5352.19775,3055.0166,444.5646,1.57952213,0,0,0,0,120,0,1),
(@OGUID+11,192312,571,1,128,5236.315,2739.21533,444.992828,-1.60570168,0,0,0,0,120,0,1),
(@OGUID+12,192313,571,1,128,5271.634,2704.829,445.182617,-3.124123,0,0,0,0,120,0,1),
(@OGUID+13,192314,571,1,128,5350.905,2622.48,444.649323,-1.56206989,0,0,0,0,120,0,1),
(@OGUID+14,192316,571,1,128,5322.013,2781.13281,435.6727,1.57952213,0,0,0,0,120,0,1),
(@OGUID+15,192317,571,1,128,5363.387,2781.27856,435.634125,1.58824873,0,0,0,0,120,0,1),
(@OGUID+16,192318,571,1,128,5322.24854,2898.94629,435.642975,-1.57952213,0,0,0,0,120,0,1),
(@OGUID+17,192319,571,1,128,5364.30371,2899.216,435.690826,-1.55334139,0,0,0,0,120,0,1),
(@OGUID+18,192320,571,1,128,5289.048,2820.22656,435.6738,0,0,0,0,0,120,0,1),
(@OGUID+19,192321,571,1,128,5288.847,2861.82128,435.590485,0.0261791088,0,0,0,0,120,0,1),
(@OGUID+20,192322,571,1,128,5322.89258,2917.14233,445.1543,1.56206989,0,0,0,0,120,0,1),
(@OGUID+21,192323,571,1,128,5364.283,2917.264,445.332184,1.58824611,0,0,0,0,120,0,1),
(@OGUID+22,192324,571,1,128,5290.514,2976.56177,435.087463,0.008724241,0,0,0,0,120,0,1),
(@OGUID+23,192325,571,1,128,5352.37744,3036.95483,435.111053,-1.56206989,0,0,0,0,120,0,1),
(@OGUID+24,192326,571,1,128,5392.64063,3036.967,433.648682,-1.51843357,0,0,0,0,120,0,1),
(@OGUID+25,192327,571,1,128,5172.336,2862.57544,435.65802,0,0,0,0,0,120,0,1),
(@OGUID+26,192328,571,1,128,5173.12842,2820.95654,435.657623,0.0261791088,0,0,0,0,120,0,1),
(@OGUID+27,192329,571,1,128,5235.32227,2924.31079,434.8981,-1.56206989,0,0,0,0,120,0,1),
(@OGUID+28,192330,571,1,128,5237.02344,2757.35669,435.625641,1.55334139,0,0,0,0,120,0,1),
(@OGUID+29,192331,571,1,128,5289.78125,2704.62158,435.714325,0.008724241,0,0,0,0,120,0,1),
(@OGUID+30,192332,571,1,128,5350.93945,2640.43066,435.2642,1.56206989,0,0,0,0,120,0,1),
(@OGUID+31,192333,571,1,128,5392.28027,2639.84033,435.207916,1.52716041,0,0,0,0,120,0,1),
(@OGUID+32,192334,571,1,128,5322.17041,2763.20142,444.9744,-1.56206715,0,0,0,0,120,0,1),
(@OGUID+33,192335,571,1,128,5363.71631,2763.24731,445.023132,-1.54461551,0,0,0,0,120,0,1),
(@OGUID+34,192487,571,1,128,5145.11133,2934.948,433.254852,-3.10665226,0,0,0,0,120,0,1),
(@OGUID+35,192487,571,1,128,5146.04443,2747.30249,433.527039,3.124123,0,0,0,0,120,0,1),
(@OGUID+36,192487,571,1,128,5158.71,2882.90161,431.27417,3.14159274,0,0,0,0,120,0,1),
(@OGUID+37,192487,571,1,128,5160.28369,2798.59766,430.6037,-3.124123,0,0,0,0,120,0,1),
(@OGUID+38,192487,571,1,128,5162.90674,2952.59766,433.368,1.57079577,0,0,0,0,120,0,1),
(@OGUID+39,192487,571,1,128,5163.85,2729.677,433.327545,-1.60570168,0,0,0,0,120,0,1),
(@OGUID+40,192487,571,1,128,5260.82471,2631.81763,433.181061,3.124123,0,0,0,0,120,0,1),
(@OGUID+41,192487,571,1,128,5262.544,3047.93018,431.96524,3.124123,0,0,0,0,120,0,1),
(@OGUID+42,192487,571,1,128,5278.43066,2613.83276,433.294434,-1.62315571,0,0,0,0,120,0,1),
(@OGUID+43,192487,571,1,128,5280.894,3064.95386,431.9758,1.55334139,0,0,0,0,120,0,1),
-- Fortress Banners "Horde"
(@OGUID+44,192269,571,1,64,4526.46,2810.18,391.2,-2.99322,0,0,0,1,180,0,1),
(@OGUID+45,192284,571,1,64,5372.48,2862.5,409.049,3.14159,0,0,0,1,180,0,1),
(@OGUID+46,192285,571,1,64,5371.49,2820.8,409.177,3.14159,0,0,0,1,180,0,1),
(@OGUID+47,192338,571,1,64,5397.76,2873.08,455.461,3.10665,0,0,0,1,180,0,1),
(@OGUID+48,192339,571,1,64,5397.39,2809.33,455.344,3.10665,0,0,0,1,180,0,1),
(@OGUID+49,192349,571,1,64,5155.31,2820.74,444.979,-3.13286,0,0,0,1,180,0,1),
(@OGUID+50,192350,571,1,64,5270.69,2861.78,445.058,-3.11539,0,0,0,1,180,0,1),
(@OGUID+51,192351,571,1,64,5271.28,2820.16,445.201,-3.13286,0,0,0,1,180,0,1),
(@OGUID+52,192352,571,1,64,5173.02,2820.93,435.72,0.017452,0,0,0,1,180,0,1),
(@OGUID+53,192353,571,1,64,5172.11,2862.57,435.721,0.017452,0,0,0,1,180,0,1),
(@OGUID+54,192354,571,1,64,5288.41,2861.79,435.721,0.017452,0,0,0,1,180,0,1),
(@OGUID+55,192355,571,1,64,5288.92,2820.22,435.721,0.017452,0,0,0,1,180,0,1),
(@OGUID+56,192356,571,1,64,5237.07,2757.03,435.796,1.51844,0,0,0,1,180,0,1),
(@OGUID+57,192357,571,1,64,5235.34,2924.34,435.04,-1.5708,0,0,0,1,180,0,1),
(@OGUID+58,192358,571,1,64,5322.23,2899.43,435.808,-1.58825,0,0,0,1,180,0,1),
(@OGUID+59,192359,571,1,64,5364.35,2899.4,435.839,-1.5708,0,0,0,1,180,0,1),
(@OGUID+60,192360,571,1,64,5352.37,3037.09,435.252,-1.5708,0,0,0,1,180,0,1),
(@OGUID+61,192361,571,1,64,5392.65,3037.11,433.713,-1.52716,0,0,0,1,180,0,1),
(@OGUID+62,192362,571,1,64,5322.12,2763.61,444.974,-1.55334,0,0,0,1,180,0,1),
(@OGUID+63,192363,571,1,64,5363.61,2763.39,445.024,-1.54462,0,0,0,1,180,0,1),
(@OGUID+64,192364,571,1,64,5350.88,2622.72,444.686,-1.5708,0,0,0,1,180,0,1),
(@OGUID+65,192366,571,1,64,5236.27,2739.46,444.992,-1.59698,0,0,0,1,180,0,1),
(@OGUID+66,192367,571,1,64,5271.8,2704.87,445.183,-3.13286,0,0,0,1,180,0,1),
(@OGUID+67,192368,571,1,64,5289.46,2704.68,435.875,-0.017451,0,0,0,1,180,0,1),
(@OGUID+68,192369,571,1,64,5350.95,2640.36,435.408,1.5708,0,0,0,1,180,0,1),
(@OGUID+69,192370,571,1,64,5392.27,2639.74,435.331,1.50971,0,0,0,1,180,0,1),
(@OGUID+70,192371,571,1,64,5364.29,2916.94,445.331,1.57952,0,0,0,1,180,0,1),
(@OGUID+71,192372,571,1,64,5322.86,2916.95,445.154,1.56207,0,0,0,1,180,0,1),
(@OGUID+72,192373,571,1,64,5290.35,2976.56,435.221,0.017452,0,0,0,1,180,0,1),
(@OGUID+73,192374,571,1,64,5272.94,2976.55,444.492,3.12412,0,0,0,1,180,0,1),
(@OGUID+74,192375,571,1,64,5235.19,2941.9,444.278,1.58825,0,0,0,1,180,0,1),
(@OGUID+75,192376,571,1,64,5352.19775,3055.0166,444.5646,1.57952,0,0,0,0,120,0,1),
(@OGUID+76,192377,571,1,64,5414.19,3069.8,415.187,1.64061,0,0,0,1,180,0,1),
(@OGUID+77,192378,571,1,64,5322.02,2781.13,435.811,1.5708,0,0,0,1,180,0,1),
(@OGUID+78,192379,571,1,64,5363.42,2781.03,435.763,1.5708,0,0,0,1,180,0,1),
(@OGUID+79,192254,571,1,64,5154.46,2828.94,409.189,3.14159,0,0,0,1,180,0,1),
(@OGUID+80,192255,571,1,64,5154.52,2853.31,409.183,3.14159,0,0,0,1,180,0,1),
(@OGUID+81,192336,571,1,64,5154.49,2862.15,445.012,3.14159,0,0,0,1,180,0,1),
(@OGUID+82,192488,571,1,64,5160.34,2798.61,430.769,3.14159,0,0,0,1,180,0,1),
(@OGUID+83,192488,571,1,64,5158.81,2883.13,431.618,3.14159,0,0,0,1,180,0,1),
(@OGUID+84,192488,571,1,64,5278.38,2613.83,433.409,-1.58825,0,0,0,1,180,0,1),
(@OGUID+85,192488,571,1,64,5260.82,2631.8,433.324,3.05433,0,0,0,1,180,0,1),
(@OGUID+86,192488,571,1,64,5163.13,2952.59,433.503,1.53589,0,0,0,1,180,0,1),
(@OGUID+87,192488,571,1,64,5145.11,2935,433.386,3.14159,0,0,0,1,180,0,1),
(@OGUID+88,192488,571,1,64,5262.54,3047.95,432.055,3.10665,0,0,0,1,180,0,1),
(@OGUID+89,192488,571,1,64,5146.04,2747.21,433.584,3.07177,0,0,0,1,180,0,1),
(@OGUID+90,192488,571,1,64,5163.78,2729.68,433.394,-1.58825,0,0,0,1,180,0,1),
(@OGUID+91,192488,571,1,64,5280.894,3064.95386,431.9758,1.55334139,0,0,0,0,120,0,1),
-- Tower Banners "Alliance"
(@OGUID+92,192501,571,1,64,4398.82,2804.7,429.792,-1.58825,0,0,0,1,180,0,1),
(@OGUID+93,192501,571,1,64,4416,2822.67,429.851,-0.017452,0,0,0,1,180,0,1),
(@OGUID+94,192501,571,1,64,4559.11,3606.22,419.999,-1.48353,0,0,0,1,180,0,1),
(@OGUID+95,192501,571,1,64,4539.42,3622.49,420.034,-3.07177,0,0,0,1,180,0,1),
(@OGUID+96,192501,571,1,64,4555.26,3641.65,419.974,1.67551,0,0,0,1,180,0,1),
(@OGUID+97,192501,571,1,64,4574.87,3625.91,420.079,0.087266,0,0,0,1,180,0,1),
(@OGUID+98,192501,571,1,64,4466.79,1960.42,459.144,1.15192,0,0,0,1,180,0,1),
(@OGUID+99,192501,571,1,64,4475.35,1937.03,459.07,-0.436332,0,0,0,1,180,0,1),
(@OGUID+100,192501,571,1,64,4451.76,1928.1,459.076,-2.00713,0,0,0,1,180,0,1),
(@OGUID+101,192501,571,1,64,4442.99,1951.9,459.093,2.74016,0,0,0,1,180,0,1),
(@OGUID+102,192501,571,1,64,4380.36328,2822.38013,429.8818,-3.106652,0,0,0,0,120,0,1),
(@OGUID+103,192501,571,1,64,4397.6626,2840.299,429.921661,1.58824873,0,0,0,0,120,0,1),
-- Tower Banners "Horde"
(@OGUID+104,192502,571,1,128,4398.82,2804.7,429.792,-1.58825,0,0,0,1,180,0,1),
(@OGUID+105,192502,571,1,128,4416,2822.67,429.851,-0.017452,0,0,0,1,180,0,1),
(@OGUID+106,192502,571,1,128,4559.11,3606.22,419.999,-1.48353,0,0,0,1,180,0,1),
(@OGUID+107,192502,571,1,128,4539.42,3622.49,420.034,-3.07177,0,0,0,1,180,0,1),
(@OGUID+108,192502,571,1,128,4555.26,3641.65,419.974,1.67551,0,0,0,1,180,0,1),
(@OGUID+109,192502,571,1,128,4574.87,3625.91,420.079,0.087266,0,0,0,1,180,0,1),
(@OGUID+110,192502,571,1,128,4466.79,1960.42,459.144,1.15192,0,0,0,1,180,0,1),
(@OGUID+111,192502,571,1,128,4475.35,1937.03,459.07,-0.436332,0,0,0,1,180,0,1),
(@OGUID+112,192502,571,1,128,4451.76,1928.1,459.076,-2.00713,0,0,0,1,180,0,1),
(@OGUID+113,192502,571,1,128,4442.99,1951.9,459.093,2.74016,0,0,0,1,180,0,1),
(@OGUID+114,192502,571,1,128,4380.36328,2822.38013,429.8818,-3.106652,0,0,0,0,120,0,1),
(@OGUID+115,192502,571,1,128,4397.6626,2840.299,429.921661,1.58824873,0,0,0,0,120,0,1);

SET @OGUID := 280000;
DELETE FROM `gameobject` WHERE `id` IN (192458,192459,192460,192461,192289,192290,192434,192435,192280,192283,192425,192426,192427,192428,192288,192291,192400,192401,192281,192282);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
-- The Sunken Ring "Horde"
(@OGUID+0,192458,571,1,16,4811.4,2441.9,358.207,-2.0333,0,0,0,1,180,0,1),
(@OGUID+1,192459,571,1,16,4805.67,2407.48,358.191,1.78023,0,0,0,1,180,0,1),
(@OGUID+2,192460,571,1,16,5004.35,2486.36,358.449,2.17294,0,0,0,1,180,0,1),
(@OGUID+3,192461,571,1,16,4983.28,2503.09,358.177,-0.427603,0,0,0,1,180,0,1),
(@OGUID+4,192289,571,1,16,4778.19,2438.06,345.644,-2.94088,0,0,0,1,180,0,1),
(@OGUID+5,192290,571,1,16,5024.57,2532.75,344.023,-1.93732,0,0,0,1,180,0,1),
-- The Sunken Ring "Alliance"
(@OGUID+6,192425,571,1,32,4811.435,2441.84546,357.982483,-2.02457881,0,0,0,0,120,0,1),
(@OGUID+7,192426,571,1,32,4805.514,2407.84375,357.940765,1.7715075,0,0,0,0,120,0,1),
(@OGUID+8,192427,571,1,32,5004.35,2486.36,358.449,2.17294,0,0,0,1,180,0,1),
(@OGUID+9,192428,571,1,32,4983.221,2503.27271,357.959534,-0.43633157,0,0,0,0,120,0,1),
(@OGUID+10,192288,571,1,32,4778.065,2438.02441,345.7063,-2.932139,0,0,0,0,120,0,1),
(@OGUID+11,192291,571,1,32,5024.608,2532.72583,344.4308,-1.94604158,0,0,0,0,120,0,1),
-- The Broken Temple "Horde"
(@OGUID+12,192434,571,1,16,5041.61,3294.4,382.15,-1.63188,0,0,0,1,180,0,1),
(@OGUID+13,192435,571,1,16,4855.63,3297.62,376.739,-3.13286,0,0,0,1,180,0,1),
(@OGUID+14,192280,571,1,16,4857.97,3335.44,368.881,-2.94959,0,0,0,1,180,0,1),
(@OGUID+15,192283,571,1,16,5006.34,3280.4,371.163,2.22529,0,0,0,1,180,0,1),
-- The Broken Temple "Alliance"
(@OGUID+16,192400,571,1,32,5041.650879,3294.318604,381.919952,-1.605702,0,0,0,1,180,0,1),
(@OGUID+17,192401,571,1,32,4855.444336,3297.600830,376.495758,-3.115388,0,0,0,1,180,0,1),
(@OGUID+18,192281,571,1,32,4857.971191,3335.415771,369.291901,-2.888511,0,0,0,1,180,0,1),
(@OGUID+19,192282,571,1,32,5006.322754,3280.362061,371.242249,2.242746,0,0,0,1,180,0,1);

-- EVERYTHING UNDER HERE IS WIP
/*
-- Eastspark Workshop "Horde"
(@OGUID+20,192452,571,1,16,4416.7993,2414.0383,377.4869,0.0087,0,0,0,0,120,0,1),
-- Eastspark Workshop "Alliance"
(@OGUID+21,192416,571,1,32,4408.57,2422.61,377.179,1.58825,0,0,0,1,180,0,1),
(@OGUID+22,192417,571,1,32,4416.59,2414.08,377.196,0,0,0,0,1,180,0,1),
(@OGUID+23,192418,571,1,32,4417.25,2301.14,377.214,0.026179,0,0,0,1,180,0,1),
(@OGUID+24,192273,571,1,32,4417.94,2324.81,371.577,3.08051,0,0,0,1,180,0,1),
-- Westspark Workshop "Horde"

-- Westspark Workshop "Alliance"
(@OGUID+25,192274,571,1,32,4424.15,3286.54,371.546,3.12412,0,0,0,1,180,0,1),
(@OGUID+26,192406,571,1,32,4438.3,3361.08,371.568,-0.017451,0,0,0,1,180,0,1),
(@OGUID+27,192407,571,1,32,4448.17,3235.63,370.412,-1.56207,0,0,0,1,180,0,1),
(@OGUID+28,192433,571,1,32,4401.63,3377.46,363.365,1.55334,0,0,0,1,180,0,1),

-- *** in progress zone horde banner in phase 128 + 32

-- Winters Edge tower alliance
(192429,571,1,64,4464.123535 2855.453125 406.110596,0.829032,0,0,0,0,120,0,1);
(192430,571,1,64,4434.555664,2883.448730,406.025299,0.759217,0,0,0,0,120,0,1);
(192431,571,1,64,4349.900391,2885.561768,406.104889,1.605702,0,0,0,0,120,0,1);
(192414,571,1,64,4387.621582 2719.565918 389.935120,-1.544616,0,0,0,0,120,0,1);
-- Winters Edge tower horde
(192442,571,1,128,4350.039,2885.61377,406.3287,1.58824873,0,0,0,0,120,0,1);
(192443,571,1,128,4434.32568,2883.244,406.3456,0.767943859,0,0,0,0,120,0,1);
(192444,571,1,128,4464.17432,2855.3186,406.390961,0.802850962,0,0,0,0,120,0,1);
(192450,571,1,128,4387.59033,2719.90015,390.200531,-1.51843357,0,0,0,0,120,0,1);

(192449,571,1,129,4517.75342,2717.231,387.811981,-1.53588688,0,0,0,0,120,0,1);
(192424,571,1,417,4563.70166,2171.03125,367.607056,-1.82386875,0,0,0,0,120,0,1),
(192626,571,1,65,4938.364,2454.59863,320.391785,-2.513274,0,0,0,0,120,0,1);
(192627,571,1,65,4939.48535,3339.65259,376.874939,0.890116632,0,0,0,0,120,0,1);

192271 4424.741211 2975.604980 367.226807 1.701696 -- banner pole shawdowsight
*/

-- Misc objects in fortress phased properly
SET @OGUID := 282000;
DELETE FROM `gameobject` WHERE `guid` IN (67165,67224,67222,67190,67195,67216,67193,67182,67186,67192,67161,67164,67180,67187);
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID AND @OGUID+55;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(@OGUID+0,193096,571,1,128,5379.885,3008.093,409.181915,-3.124123,0,0,0,0,120,0,1),
(@OGUID+1,193097,571,1,128,5381.73975,3008.15454,409.181915,2.98449826,0,0,0,0,120,0,1),
(@OGUID+2,193098,571,1,128,5383.672,3008.02783,409.181915,-3.115388,0,0,0,0,120,0,1),
(@OGUID+3,193099,571,1,128,5386.25342,3007.79614,409.181915,2.932139,0,0,0,0,120,0,1),
(@OGUID+4,193100,571,1,128,5387.354,3009.64941,409.181915,-1.30899549,0,0,0,0,120,0,1),
(@OGUID+5,193101,571,1,128,5381.12744,3010.09717,409.181915,-2.72271276,0,0,0,0,120,0,1),
(@OGUID+6,193102,571,1,128,5383.12061,3007.90967,410.8231,-2.530723,0,0,0,0,120,0,1),
(@OGUID+7,193103,571,1,128,5381.105,3007.89575,410.8231,-3.09791875,0,0,0,0,120,0,1),
(@OGUID+8,193104,571,1,128,5376.777,3010.619,409.191742,-2.60926127,0,0,0,0,120,0,1),
(@OGUID+9,193105,571,1,128,5381.47559,3010.24731,410.8231,-2.80997539,0,0,0,0,120,0,1),
(@OGUID+10,193106,571,1,128,5381.059,3009.85864,410.8231,2.66161919,0,0,0,0,120,0,1),
(@OGUID+11,193107,571,1,128,5381.038,3010.44263,410.8157,-2.0507617,0,0,0,0,120,0,1),
(@OGUID+12,193108,571,1,128,5379.83154,3007.82373,410.8161,-2.02457881,0,0,0,0,120,0,1),
(@OGUID+13,193109,571,1,128,5379.99463,3008.40356,410.815918,-3.03687477,0,0,0,0,120,0,1),
(@OGUID+14,193124,571,1,128,5293.65869,2924.44019,409.29306,1.20427489,0,0,0,0,120,0,1),
(@OGUID+15,193125,571,1,1,5293.28,2932.32813,409.065247,-2.49581814,0,0,0,0,120,0,1),
(@OGUID+16,193126,571,1,1,5292.30469,2930.5105,409.157135,-3.06302428,0,0,0,0,120,0,1),
(@OGUID+17,193127,571,1,64,5293.349,2923.712,409.844757,-1.8762306,0,0,0,0,120,0,1),
(@OGUID+18,193128,571,1,128,5293.12256,2895.22754,409.208771,-0.9861096,0,0,0,0,120,0,1),
(@OGUID+19,193129,571,1,128,5292.913,2895.54346,410.419617,-0.122171074,0,0,0,0,120,0,1),
(@OGUID+20,193130,571,1,128,5294.09473,2894.191,409.164063,-0.7330382,0,0,0,0,120,0,1),
(@OGUID+21,193131,571,1,128,5295.1875,2895.382,409.143219,-0.349065244,0,0,0,0,120,0,1),
(@OGUID+22,193132,571,1,128,5294.527,2895.57471,410.6591,-1.92858779,0,0,0,0,120,0,1),
(@OGUID+23,193133,571,1,128,5295.3916,2895.05737,410.6686,0.6894028,0,0,0,0,120,0,1),
(@OGUID+24,193134,571,1,128,5295.13525,2895.68481,410.618866,-2.22529364,0,0,0,0,120,0,1),
(@OGUID+25,193135,571,1,128,5294.97559,2895.33521,410.657684,-2.73143482,0,0,0,0,120,0,1),
(@OGUID+26,193136,571,1,128,5293.22559,2895.46436,410.413483,-0.802850962,0,0,0,0,120,0,1),
(@OGUID+27,193137,571,1,128,5295.56,2895.24146,410.628052,-2.11184788,0,0,0,0,120,0,1),
(@OGUID+28,193138,571,1,128,5293.741,2894.48169,409.183167,-2.72271276,0,0,0,0,120,0,1),
(@OGUID+29,193139,571,1,64,5294.599,2786.85254,409.8877,-2.356195,0,0,0,0,120,0,1),
(@OGUID+30,193140,571,1,64,5294.37939,2785.03833,409.175018,-2.33873963,0,0,0,0,120,0,1),
(@OGUID+31,193141,571,1,64,5293.205,2787.03052,409.218872,3.03687477,0,0,0,0,120,0,1),
(@OGUID+32,193142,571,1,64,5294.241,2786.42456,409.174347,0.0174524616,0,0,0,0,120,0,1),
(@OGUID+33,193143,571,1,64,5291.705,2785.86646,409.282135,-2.03330517,0,0,0,0,120,0,1),
(@OGUID+34,193144,571,1,64,5293.03369,2785.632,409.22522,-1.2915417,0,0,0,0,120,0,1),
(@OGUID+35,193145,571,1,64,5295.866,2787.7666,409.1923,2.155478,0,0,0,0,120,0,1),
(@OGUID+36,193146,571,1,64,5293.56445,2787.31079,410.55954,0.261798173,0,0,0,0,120,0,1),
(@OGUID+37,193147,571,1,128,5233.12061,2920.362,409.163544,-0.7243115,0,0,0,0,120,0,1),
(@OGUID+38,193148,571,1,128,5238.27539,2920.67358,409.256439,-0.418878615,0,0,0,0,120,0,1),
(@OGUID+39,193149,571,1,128,5235.902,2920.751,409.224457,-0.951203167,0,0,0,0,120,0,1),
(@OGUID+40,193150,571,1,128,5237.36963,2919.89771,409.556641,0.8202983,0,0,0,0,120,0,1),
(@OGUID+41,193151,571,1,128,5234.19775,2918.99731,409.322754,-2.33873963,0,0,0,0,120,0,1),
(@OGUID+42,193152,571,1,128,5234.52344,2921.76221,409.175781,-2.2165668,0,0,0,0,120,0,1),
(@OGUID+43,193153,571,1,128,5234.119,2918.93921,409.1339,-3.098036,0,0,0,0,120,0,1),
(@OGUID+44,193154,571,1,128,5234.26758,2919.40015,409.502869,-2.18166113,0,0,0,0,120,0,1),
(@OGUID+45,193155,571,1,128,5293.37939,2746.05566,409.22052,-0.06981169,0,0,0,0,120,0,1),
(@OGUID+46,193156,571,1,128,5293.65039,2755.67529,409.1913,-0.43633157,0,0,0,0,120,0,1),
(@OGUID+47,193157,571,1,128,5292.23535,2753.59473,409.0867,-0.357789934,0,0,0,0,120,0,1),
(@OGUID+48,193158,571,1,128,5292.42969,2748.62427,409.131042,0.253072351,0,0,0,0,120,0,1),
(@OGUID+49,193159,571,1,128,5293.384,2750.90283,409.234924,-0.0610866137,0,0,0,0,120,0,1),
(@OGUID+50,193160,571,1,64,5371.89746,2805.47583,409.3072,0.0610866137,0,0,0,0,120,0,1),
(@OGUID+51,193161,571,1,64,5376.616,2875.105,409.254822,1.59697616,0,0,0,0,120,0,1),
(@OGUID+52,193162,571,1,128,5377.54932,2870.92456,409.239166,-0.549776852,0,0,0,0,120,0,1),
(@OGUID+53,193163,571,1,128,5378.068,2813.61719,409.239166,1.55334139,0,0,0,0,120,0,1),
(@OGUID+54,193164,571,1,128,5378.921,2805.43677,409.239166,1.53588688,0,0,0,0,120,0,1),
(@OGUID+55,193165,571,1,128,5378.452,2876.67456,409.239166,1.54461825,0,0,0,0,120,0,1);
