#include "LoadGame.h"

void loadEnemies(Enemy* Enemies[])
{
	Enemies[0] = initialize_enemy(420, 412 , 352, 575, 40, 64);
	Enemies[1] = initialize_enemy(440, 576, 350, 579, 40, 64);
	Enemies[2] = initialize_enemy(1000, 576,  780, 1050, 40, 64);
	Enemies[3] = initialize_enemy(1554, 576, 1406, 1573, 40, 64);
	Enemies[4] = initialize_enemy(1800, 412, 1761, 1909, 40, 64);
	Enemies[5] = initialize_enemy(2880, 576, 2764, 2976, 40, 64);
	Enemies[6] = initialize_enemy(2539, 576, 2539, 2720, 40, 64);
	Enemies[7] = initialize_enemy(4320, 412, 4260, 4488, 40, 64);
	Enemies[8] = initialize_enemy(4710, 448, 4654, 4808, 40, 64);

}

void loadPlatforms(Platform* Platforms[])
{
	Platforms[0] = new_platform(320, 476, 320, 36);
	Platforms[1] = new_platform(1380, 520, 256, 36);
	Platforms[2] = new_platform(1728, 476, 256, 36);
	Platforms[3] = new_platform(2752, 512, 256, 36);
	Platforms[4] = new_platform(3188, 476, 384, 36);
	Platforms[5] = new_platform(4219, 476, 320, 36);
	Platforms[6] = new_platform(4603, 512, 256, 36);
}

void loadSpikes(Spike* Spikes[])
{
	Spikes[0] = new_spikes(1175, 608, 22, 22);
	Spikes[1] = new_spikes(1148, 608, 22, 22);
	Spikes[2] = new_spikes(1121, 608, 22, 22);
	Spikes[3] = new_spikes(2114, 608, 22, 22);
	Spikes[4] = new_spikes(2141, 608, 22, 22);
	Spikes[5] = new_spikes(2168, 608, 22, 22);
	Spikes[6] = new_spikes(2195, 608, 22, 22);
	Spikes[7] = new_spikes(3008, 608, 22, 22);
	Spikes[8] = new_spikes(3031, 608, 22, 22);
	Spikes[9] = new_spikes(3058, 608, 22, 22);
	Spikes[10] = new_spikes(3085, 608, 22, 22);
	Spikes[11] = new_spikes(3112, 608, 22, 22);
	Spikes[12] = new_spikes(3139, 608, 22, 22);
	Spikes[13] = new_spikes(3166, 608, 22, 22);
	Spikes[14] = new_spikes(4084, 608, 22, 22);
	Spikes[15] = new_spikes(4111, 608, 22, 22);
	Spikes[16] = new_spikes(4138, 608, 22, 22);
	Spikes[17] = new_spikes(4165, 608, 22, 22);
	Spikes[18] = new_spikes(4192, 608, 22, 22);
	Spikes[19] = new_spikes(4219, 608, 22, 22);
	Spikes[20] = new_spikes(4246, 608, 22, 22);
	Spikes[21] = new_spikes(1477, 490, 22, 22);
	Spikes[22] = new_spikes(1504, 490, 22, 22);

	Spikes[23] = new_spikes(5309, 608, 22, 22);
	Spikes[24] = new_spikes(5282, 608, 22, 22);
	Spikes[25] = new_spikes(5255, 608, 22, 22);
	Spikes[26] = new_spikes(5228, 608, 22, 22);
	Spikes[27] = new_spikes(5201, 608, 22, 22);

}

void loadCoins(Coin* Coins[])
{
	Coins[0] = new_coin(355, 376, 22, 22);
	Coins[1] = new_coin(405, 376, 22, 22);
	Coins[2] = new_coin(455, 376, 22, 22);
	Coins[3] = new_coin(505, 376, 22, 22);
	Coins[4] = new_coin(555, 376, 22, 22);
	Coins[5] = new_coin(605, 376, 22, 22);
	Coins[6] = new_coin(1740, 412, 22, 22);
	Coins[7] = new_coin(1952, 412, 22, 22);
	Coins[8] = new_coin(1909, 379, 22, 22);
	Coins[9] = new_coin(1866, 335, 22, 22);
	Coins[10] = new_coin(1819, 335, 22, 22);
	Coins[11] = new_coin(1781, 376, 22, 22);
	Coins[12] = new_coin(1220, 343, 22, 22);
	Coins[13] = new_coin(1220, 298, 22, 22);
	Coins[14] = new_coin(1173, 324, 22, 22);
	Coins[15] = new_coin(1263, 324, 22, 22);
	Coins[16] = new_coin(1263, 365, 22, 22);
	Coins[17] = new_coin(1173, 365, 22, 22);
	Coins[18] = new_coin(1220, 390, 22, 22);
	Coins[19] = new_coin(2800, 412, 22, 22);
	Coins[20] = new_coin(2850, 412, 22, 22);
	Coins[21] = new_coin(2900, 412, 22, 22);
	Coins[22] = new_coin(2950, 412, 22, 22);
	Coins[23] = new_coin(3250, 412, 22, 22);
	Coins[24] = new_coin(3300, 412, 22, 22);
	Coins[25] = new_coin(3350, 412, 22, 22);
	Coins[26] = new_coin(3400, 412, 22, 22);
	Coins[27] = new_coin(3450, 412, 22, 22);
	Coins[28] = new_coin(3500, 412, 22, 22);
	Coins[29] = new_coin(3250, 586, 22, 22);
	Coins[30] = new_coin(3300, 586, 22, 22);
	Coins[31] = new_coin(3350, 586, 22, 22);
	Coins[32] = new_coin(3400, 586, 22, 22);
	Coins[33] = new_coin(3450, 586, 22, 22);
	Coins[34] = new_coin(3500, 586, 22, 22);
	Coins[35] = new_coin(3851, 412, 22, 22);
	Coins[36] = new_coin(3924, 412, 22, 22);
	Coins[37] = new_coin(3890, 412, 22, 22);
	Coins[38] = new_coin(3922, 547, 22, 22);
	Coins[39] = new_coin(3881, 547, 22, 22);
	Coins[40] = new_coin(3844, 547, 22, 22);
	Coins[41] = new_coin(3933, 597, 22, 22);
	Coins[42] = new_coin(3879, 597, 22, 22);
	Coins[43] = new_coin(3829, 597, 22, 22);
	Coins[44] = new_coin(4967, 428, 22, 22);
	Coins[45] = new_coin(4967, 478, 22, 22);
	Coins[46] = new_coin(5017, 378, 22, 22);
	Coins[47] = new_coin(5017, 428, 22, 22);
	Coins[48] = new_coin(5017, 478, 22, 22);
	Coins[49] = new_coin(5017, 528, 22, 22);
	Coins[50] = new_coin(5067, 378, 22, 22);
	Coins[51] = new_coin(5067, 428, 22, 22);
	Coins[52] = new_coin(5067, 478, 22, 22);
	Coins[53] = new_coin(5067, 528, 22, 22);
	Coins[54] = new_coin(5117, 428, 22, 22);
	Coins[55] = new_coin(5117, 478, 22, 22);
}

void loadDBoxes(DestructableBox* DBox[])
{
	DBox[0] = new_dbox(1199, 576, 64, 64);
	DBox[1] = new_dbox(1199, 512, 64, 64);
	DBox[2] = new_dbox(1199, 448, 64, 64);
	DBox[3] = new_dbox(2432, 320, 64, 64);
	DBox[4] = new_dbox(2432, 384, 64, 64);
	DBox[5] = new_dbox(3956, 512, 64, 64);
	DBox[6] = new_dbox(4020, 576, 64, 64);
	DBox[7] = new_dbox(3956, 576, 64, 64);
}

void loadBoxes(Box* Boxes[])
{
	Boxes[0] = new_box(2368, 512, 64, 64);
	Boxes[1] = new_box(2432, 448, 64, 64);
	Boxes[2] = new_box(2432, 512, 64, 64);
	Boxes[3] = new_box(2432, 576, 64, 64);
	Boxes[4] = new_box(2304, 576, 64, 64);
	Boxes[5] = new_box(2368, 576, 64, 64);
	Boxes[6] = new_box(2432, 256, 64, 64);
	Boxes[7] = new_box(0, 512, 64, 64);
	Boxes[8] = new_box(0, 576, 64, 64);
	Boxes[9] = new_box(0, 448, 64, 64);
	Boxes[10] = new_box(0, 384, 64, 64);
	Boxes[11] = new_box(0, 320, 64, 64);
	Boxes[12] = new_box(0, 256, 64, 64);
	Boxes[13] = new_box(0, 192, 64, 64);
	Boxes[14] = new_box(0, 128, 64, 64);
	Boxes[15] = new_box(3700, 576, 64, 64);
	Boxes[16] = new_box(3764, 512, 64, 64);
	Boxes[17] = new_box(3828, 448, 64, 64);
	Boxes[18] = new_box(3892, 448, 64, 64);
	Boxes[19] = new_box(5437, 576, 64, 64);
	Boxes[20] = new_box(5501, 576, 64, 64);
	Boxes[21] = new_box(5565, 576, 64, 64);
	Boxes[22] = new_box(5629, 576, 64, 64);
	Boxes[23] = new_box(5501, 512, 64, 64);
	Boxes[24] = new_box(5565, 512, 64, 64);
	Boxes[25] = new_box(5629, 512, 64, 64);
	Boxes[26] = new_box(5565, 448, 64, 64);
	Boxes[27] = new_box(5629, 448, 64, 64);
	Boxes[28] = new_box(5629, 384, 64, 64);
	Boxes[29] = new_box(3764, 576, 64, 64);
}

void loadClouds(Cloud* Clouds[])
{
	Clouds[0] = new_cloud(431, 90);
	Clouds[1] = new_cloud(301, 150);
	Clouds[2] = new_cloud(224, 74);
	Clouds[3] = new_cloud(80, 140);
	Clouds[4] = new_cloud(533, 140);
	Clouds[5] = new_cloud(645, 74);
	Clouds[6] = new_cloud(762, 150);
	Clouds[7] = new_cloud(962, 140);
	Clouds[8] = new_cloud(1079, 74);
	Clouds[9] = new_cloud(1223, 150);
	Clouds[10] = new_cloud(1300, 90);
	Clouds[11] = new_cloud(1430, 140);
	Clouds[12] = new_cloud(1562, 74);
	Clouds[13] = new_cloud(1644, 150);
	Clouds[14] = new_cloud(1844, 140);
	Clouds[15] = new_cloud(1961, 74);
	Clouds[16] = new_cloud(2105, 150);
	Clouds[17] = new_cloud(2262, 90);
	Clouds[18] = new_cloud(2364, 140);
	Clouds[19] = new_cloud(2466, 74);
	Clouds[20] = new_cloud(2578, 150);
	Clouds[21] = new_cloud(2778, 140);
	Clouds[22] = new_cloud(2895, 74);
	Clouds[23] = new_cloud(3039, 150);
	Clouds[24] = new_cloud(3116, 90);
	Clouds[25] = new_cloud(3246, 140);
	Clouds[26] = new_cloud(3348, 74);
	Clouds[27] = new_cloud(3460, 150);
	Clouds[28] = new_cloud(3660, 140);
	Clouds[29] = new_cloud(3777, 74);
	Clouds[30] = new_cloud(3921, 150);
	Clouds[31] = new_cloud(3998, 90);
	Clouds[32] = new_cloud(4128, 140);
	Clouds[33] = new_cloud(4230, 74);
	Clouds[34] = new_cloud(4342, 150);
	Clouds[35] = new_cloud(4532, 140);
	Clouds[36] = new_cloud(4659, 74);
	Clouds[37] = new_cloud(4803, 150);
	Clouds[38] = new_cloud(4880, 90);
	Clouds[39] = new_cloud(5010, 140);
	Clouds[40] = new_cloud(5112, 74);
	Clouds[41] = new_cloud(5224, 150);
	Clouds[42] = new_cloud(5424, 140);
	Clouds[43] = new_cloud(5541, 74);
	Clouds[44] = new_cloud(5618, 150);
	Clouds[45] = new_cloud(5748, 90);
	Clouds[46] = new_cloud(5850, 140);
	Clouds[47] = new_cloud(6050, 140);
	Clouds[48] = new_cloud(6167, 74);
	Clouds[49] = new_cloud(6311, 150);
	Clouds[50] = new_cloud(6388, 90);
	Clouds[51] = new_cloud(6518, 140);
	Clouds[52] = new_cloud(6620, 90);
	Clouds[53] = new_cloud(6732, 140);
}
