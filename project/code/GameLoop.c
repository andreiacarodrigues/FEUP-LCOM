#include "GameLoop.h"

typedef enum { Main_Menu, Game, Controllers, High_Scores, Credits, Game_Over } Game_State;

void startGame(unsigned short mode)
{
	/* Loads Scores */
	Score* Scores[5];
	init_scores(Scores);
	read_scores(Scores);
	int score_show = 0;

	/* Get the date from RTC */

	RTC* rtc = newRTC();

	/* Initiates Graphic Mode */

	if(lm_init() == NULL)
		return;

	char *video_mem;
	video_mem = initVideoGraphics(mode);

	if(video_mem == NULL)
		return;

	/* Initiates Keyboard Interruptions */

	int control_kbd = kbd_subscribe_int();

	if (control_kbd < 0)
		return;

	/* Initiates Timer Interruptions */

	int control_timer = timer_subscribe_int();

	if (control_timer < 0)
		return;

	/* Initiates Mouse Interruptions */

	int control_mouse = mouse_subscribe_int();

	if (control_mouse < 0)
	{
		printf("\n mouse_subscribe_int() failure \n");
		return;
	}

	if(mouse_set_stream_mode())
		return;

	if(mouse_enable_stream_mode())
		return;

	/* Initiates System Variables */

	int ipc_status, r;
	int irq_set_kbd = control_kbd;
	int irq_set_timer = BIT(control_timer);
	int irq_set_mouse = control_mouse;
	message msg;
	int running = 1; /* for the keyboard interruptions */
	int counter = 0; /* for the timer interruptions */
	unsigned int i;
	unsigned int j;

	/* Initiates Main Menu Variables */

	Game_State myGameState = Main_Menu;

	// Main Menu Images
	Bitmap* MenuScreen1 = loadBitmap(path("Menu1"));
	Bitmap* MenuScreen2 = loadBitmap(path("Menu2"));
	Bitmap* MenuScreen3 = loadBitmap(path("Menu3"));
	Bitmap* MenuScreen4 = loadBitmap(path("Menu4"));

	// Credits Images
	Bitmap* ControllersScreen1 = loadBitmap(path("Controllers1"));
	Bitmap* ControllersScreen2 = loadBitmap(path("Controllers2"));
	Bitmap* ControllersScreen3 = loadBitmap(path("Controllers3"));
	Bitmap* ControllersScreen4 = loadBitmap(path("Controllers4"));

	// Credits Images
	Bitmap* CreditsScreen1 = loadBitmap(path("Credits1"));
	Bitmap* CreditsScreen2 = loadBitmap(path("Credits2"));
	Bitmap* CreditsScreen3 = loadBitmap(path("Credits3"));
	Bitmap* CreditsScreen4 = loadBitmap(path("Credits4"));

	// High Score Images
	Bitmap* HighScoreScreen1 = loadBitmap(path("HS1"));
	Bitmap* HighScoreScreen2 = loadBitmap(path("HS2"));
	Bitmap* HighScoreScreen3 = loadBitmap(path("HS3"));
	Bitmap* HighScoreScreen4 = loadBitmap(path("HS4"));

	// Game Over - Timed out
	Bitmap* GameOverTimedOut1 = loadBitmap(path("TO1"));
	Bitmap* GameOverTimedOut2 = loadBitmap(path("TO2"));
	Bitmap* GameOverTimedOut3 = loadBitmap(path("Died3"));
	Bitmap* GameOverTimedOut4 = loadBitmap(path("Died4"));

	// Game Over - Dead
	Bitmap* GameOverDead1 = loadBitmap(path("Died1"));
	Bitmap* GameOverDead2 = loadBitmap(path("Died2"));
	Bitmap* GameOverDead3 = loadBitmap(path("Died3"));
	Bitmap* GameOverDead4 = loadBitmap(path("Died4"));

	// Game Over
	Bitmap* GameOver1 = loadBitmap(path("GO1"));
	Bitmap* GameOver2 = loadBitmap(path("GO2"));
	Bitmap* GameOver3 = loadBitmap(path("Died3"));
	Bitmap* GameOver4 = loadBitmap(path("Died4"));

	/* Initiates Level Variables */

	/* ---------------------------------------------------------- Objects ---------------------------------------------------------- */

	// Player
	Player* myPlayer;

	// Enemies
	int nr_enemies = 9;
	Enemy* Enemies[nr_enemies];

	// Platforms
	int nr_platforms = 7;
	Platform* Platforms[nr_platforms];

	// Collectibles
	int nr_coins = 56;
	Coin* Coins[nr_coins];

	// Spikes
	int nr_spikes = 28;
	Spike* Spikes[nr_spikes];

	// Destructable Boxes
	int nr_dboxes = 8;
	DestructableBox* DBox[nr_dboxes];

	// Boxes
	int nr_boxes = 30;
	Box* Boxes[nr_boxes];

	// Clouds
	int nr_clouds = 54;
	Cloud* Clouds[nr_clouds];

	Flag* flag = new_flag(5629, 224);

	/* ---------------------------------------------------------- Sprites ---------------------------------------------------------- */

	// Cursors
	Bitmap* cursor = loadBitmap(path("cursor"));
	Bitmap* pickaxe = loadBitmap(path("pick"));

	// Player

	Bitmap* player_moving_right1 = loadBitmap(path("player_move1"));
	Bitmap* player_moving_right2 = loadBitmap(path("player_move2"));
	Bitmap* player_moving_right3 = loadBitmap(path("player_move3"));
	Bitmap* player_moving_right4 = loadBitmap(path("player_move4"));
	Bitmap* player_moving_right5 = loadBitmap(path("player_move5"));

	Bitmap* player_moving_left1 = loadBitmap(path("player_move1"));
	Bitmap* player_moving_left2 = loadBitmap(path("player_move2"));
	Bitmap* player_moving_left3 = loadBitmap(path("player_move3"));
	Bitmap* player_moving_left4 = loadBitmap(path("player_move4"));
	Bitmap* player_moving_left5 = loadBitmap(path("player_move5"));

	Bitmap* player_idle = loadBitmap(path("player"));

	flipBitmap(player_moving_left1);
	flipBitmap(player_moving_left2);
	flipBitmap(player_moving_left3);
	flipBitmap(player_moving_left4);
	flipBitmap(player_moving_left5);

	Bitmap* player_animations[11];

	player_animations[0] = player_moving_left5;
	player_animations[1] = player_moving_left4;
	player_animations[2] = player_moving_left3;
	player_animations[3] = player_moving_left2;
	player_animations[4] = player_moving_left1;
	player_animations[5] = player_idle;
	player_animations[6] = player_moving_right1;
	player_animations[7] = player_moving_right2;
	player_animations[8] = player_moving_right3;
	player_animations[9] = player_moving_right4;
	player_animations[10] = player_moving_right5;

	// Enemy

	Bitmap* enemy_moving_right1 = loadBitmap(path("skeleton_moving1"));
	Bitmap* enemy_moving_right2 = loadBitmap(path("skeleton_moving2"));
	Bitmap* enemy_moving_right3 = loadBitmap(path("skeleton_moving3"));
	Bitmap* enemy_moving_right4 = loadBitmap(path("skeleton_moving4"));
	Bitmap* enemy_moving_right5 = loadBitmap(path("skeleton_moving5"));

	Bitmap* enemy_moving_left1 = loadBitmap(path("skeleton_moving1"));
	Bitmap* enemy_moving_left2 = loadBitmap(path("skeleton_moving2"));
	Bitmap* enemy_moving_left3 = loadBitmap(path("skeleton_moving3"));
	Bitmap* enemy_moving_left4 = loadBitmap(path("skeleton_moving4"));
	Bitmap* enemy_moving_left5 = loadBitmap(path("skeleton_moving5"));

	Bitmap* enemy_idle = loadBitmap(path("enemy"));

	flipBitmap(enemy_moving_left1);
	flipBitmap(enemy_moving_left2);
	flipBitmap(enemy_moving_left3);
	flipBitmap(enemy_moving_left4);
	flipBitmap(enemy_moving_left5);

	Bitmap* enemy_animations[11];

	enemy_animations[0] = enemy_moving_left5;
	enemy_animations[1] = enemy_moving_left4;
	enemy_animations[2] = enemy_moving_left3;
	enemy_animations[3] = enemy_moving_left2;
	enemy_animations[4] = enemy_moving_left1;
	enemy_animations[5] = enemy_idle;
	enemy_animations[6] = enemy_moving_right1;
	enemy_animations[7] = enemy_moving_right2;
	enemy_animations[8] = enemy_moving_right3;
	enemy_animations[9] = enemy_moving_right4;
	enemy_animations[10] = enemy_moving_right5;

	// Terrain

	Bitmap* cloud_sprite = loadBitmap(path("cloud"));
	Bitmap* floor_sprite = loadBitmap(path("floor"));
	Bitmap* top_floor_sprite = loadBitmap(path("top_floor"));
	Bitmap* spike_sprite = loadBitmap(path("spikes"));
	Bitmap* platform_sprite = loadBitmap(path("platform"));
	Bitmap* coin_sprite = loadBitmap(path("coin"));
	Bitmap* dbox_sprite =  loadBitmap(path("special_block"));
	Bitmap* box_sprite = loadBitmap(path("block"));
	Bitmap* flag_sprite = loadBitmap(path("flag"));

	// Top Info
	Bitmap* hp_sprite = loadBitmap(path("hp"));
	Bitmap* nr1 = loadBitmap(path("1"));
	Bitmap* nr2 = loadBitmap(path("2"));
	Bitmap* nr3 = loadBitmap(path("3"));
	Bitmap* nr4 = loadBitmap(path("4"));
	Bitmap* nr5 = loadBitmap(path("5"));
	Bitmap* nr6 = loadBitmap(path("6"));
	Bitmap* nr7 = loadBitmap(path("7"));
	Bitmap* nr8 = loadBitmap(path("8"));
	Bitmap* nr9 = loadBitmap(path("9"));
	Bitmap* nr0 = loadBitmap(path("0"));
	Bitmap* sec_sprite = loadBitmap(path("s"));
	Bitmap* date_sep = loadBitmap(path("traco"));



	/* ---------------------------------------------------------- Enters the Game Loop ---------------------------------------------------------- */

	while (running)
	{
		if (r = driver_receive(ANY, &msg, &ipc_status) != 0)
		{
			printf("driver_receive() failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
				case HARDWARE:
					/* Receives Keyboard Interruption */
					if (msg.NOTIFY_ARG & irq_set_kbd)
					{
						if(kbd_int_handler(myPlayer, myGameState == Game) != 0)
							if(myGameState == Main_Menu)
								running = 0;
							else
								myGameState = Main_Menu;
					}

					/* Receive Mouse Interruption */
					if (msg.NOTIFY_ARG & irq_set_mouse)
					{
						update_mouse();
					}

					/* Receives Timer Interruption */
					if (msg.NOTIFY_ARG & irq_set_timer)
					{
						switch(myGameState)
						{
						// Main Menu
						case 0:
						{
							drawBitmap(MenuScreen1, 0, 0, ALIGN_LEFT);
							drawBitmap(MenuScreen2, 512, 0, ALIGN_LEFT);
							drawBitmap(MenuScreen3, 0, 384, ALIGN_LEFT);
							drawBitmap(MenuScreen4, 512, 384, ALIGN_LEFT);

							if(getMouse()->draw)
								drawFilteredBitmap(cursor, getMouse()->x, getMouse()->y, 38655, ALIGN_LEFT);

							if((getMouse()->x > 385 &&  getMouse()->x < 680) && (getMouse()->y > 266 &&  getMouse()->y < 332) && getMouse()->left_button_pressed)
								{
									/* Load Game */
									myPlayer = initialize_player(38,64);
									loadEnemies(Enemies);
									loadPlatforms(Platforms);
									loadCoins(Coins);
									loadSpikes(Spikes);
									loadDBoxes(DBox);
									loadBoxes(Boxes);
									loadClouds(Clouds);
									flag->x = 5629;
									myGameState = Game;
								}
							if((getMouse()->x > 313 &&  getMouse()->x < 745) && (getMouse()->y > 357 &&  getMouse()->y < 401) && getMouse()->left_button_pressed)
								myGameState = Controllers;
							if((getMouse()->x > 322 &&  getMouse()->x < 738) && (getMouse()->y > 424 &&  getMouse()->y < 469) && getMouse()->left_button_pressed)
								myGameState = High_Scores;
							if((getMouse()->x > 402 &&  getMouse()->x < 659) && (getMouse()->y > 496 &&  getMouse()->y < 539) && getMouse()->left_button_pressed)
								myGameState = Credits;
							if((getMouse()->x > 457 &&  getMouse()->x < 567) && (getMouse()->y > 567 &&  getMouse()->y < 611) && getMouse()->left_button_pressed)
								running = 0;

							break;
						}
						// Game
						case 1:
						{
							/* UPDATE */

							// Updates the player
					 		update_postion(myPlayer);
							myPlayer->time++;

							if(myPlayer->coins > 0)
								myPlayer->score = ((myPlayer->coins * 50) / (myPlayer->time / 60));

							// Checks Coins Pickup
							for(i = 0; i < nr_coins; i++)
								if(Coins[i]->active)
									coin_pickup(myPlayer, Coins[i]);

							// Checks Platform Collisions
							for(i = 0; i < nr_platforms; i++)
								check_collision_platform(myPlayer, Platforms[i]);

							// Checks Box Collisions
							for(i = 0; i < nr_boxes; i++)
								check_collision_box(myPlayer, Boxes[i]);


							// Checks DBox Collisions
							for(i = 0; i < nr_dboxes; i++)
								if(DBox[i]->active)
								{
									check_pressed_dbox(getMouse()->x, getMouse()->y, getMouse()->left_button_pressed, DBox[i]);
									check_collision_dbox(myPlayer, DBox[i]);
								}

							// Updates the enemy and Checks Collisions
							for(i = 0; i < nr_enemies; i++)
								if(Enemies[i]->active)
								{
									update_postion_enemy(Enemies[i]);
									check_collision(myPlayer, Enemies[i]);
								}

							// Checks Spikes Collisions
							for(i = 0; i < nr_spikes; i++)
								check_collision_spikes(myPlayer, Spikes[i]);

							// Checks if Game Over
							if(myPlayer->hp == 0 || myPlayer->time >= TWO_MINUTES || (myPlayer->x + myPlayer->x_size) >= flag->x)
							{
								score_show = myPlayer->score;
								myGameState = Game_Over;
							}

							// Runs a Camara update
								adjust_camara(myPlayer, Platforms, nr_platforms, Coins, nr_coins, Spikes, nr_spikes, DBox, nr_dboxes, Enemies, nr_enemies, Boxes, nr_boxes, flag, Clouds, nr_clouds);

							/* DRAW */

							//Sky
							fillDisplay(SKY);

							// Top Info - HP
							for(i = 0; i < myPlayer->hp; i++)
								drawBitmap(hp_sprite, 15 + i*40, 15, ALIGN_LEFT);

							// Top Info - Time
							int time = myPlayer->time;
							int offset = 0;
							time = time/60;

							while(time > 0)
							{
								int digit = time % 10;
								switch(digit)
								{
								case 0:
								{
									drawBitmap(nr0, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 1:
								{
									drawBitmap(nr1, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 2:
								{
									drawBitmap(nr2, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 3:
								{
									drawBitmap(nr3, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 4:
								{
									drawBitmap(nr4, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 5:
								{
									drawBitmap(nr5, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 6:
								{
									drawBitmap(nr6, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 7:
								{
									drawBitmap(nr7, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 8:
								{
									drawBitmap(nr8, 512 - offset, 20, ALIGN_LEFT);
									break;
								}
								case 9:
								{
									drawBitmap(nr9, 512 - offset, 20, ALIGN_LEFT);
									break;
								}

								}
								offset += 34;

								time /= 10;
							}
							drawBitmap(sec_sprite, 550, 20, ALIGN_LEFT);

							// Top Info - Coins
							int coins = myPlayer->coins;
							offset = 0;

							if(coins  == 0)
								drawBitmap(nr0, 950 - offset, 20, ALIGN_LEFT);
							else
							{
								while(coins > 0)
								{
									int digit = coins % 10;
									switch(digit)
									{
									case 0:
									{
										drawBitmap(nr0, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 1:
									{
										drawBitmap(nr1, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 2:
									{
										drawBitmap(nr2, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 3:
									{
										drawBitmap(nr3, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 4:
									{
										drawBitmap(nr4, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 5:
									{
										drawBitmap(nr5, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 6:
									{
										drawBitmap(nr6, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 7:
									{
										drawBitmap(nr7, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 8:
									{
										drawBitmap(nr8, 950 - offset, 20, ALIGN_LEFT);
										break;
									}
									case 9:
									{
										drawBitmap(nr9, 950 - offset, 20, ALIGN_LEFT);
										break;
									}

									}
								offset += 34;

								coins /= 10;
								}
							}
							drawBitmap(coin_sprite, 987, 27, ALIGN_LEFT);

							// Coins
							for(i = 0; i < nr_coins; i++)
								if(Coins[i]->active)
									drawBitmap(coin_sprite, Coins[i]->x, Coins[i]->y, ALIGN_LEFT);

							// Clouds
							for(i = 0; i < nr_clouds; i++)
								drawBitmap(cloud_sprite, Clouds[i]->x, Clouds[i]->y, ALIGN_LEFT);

							// Spikes
							for(i = 0; i < nr_spikes; i++)
								drawBitmap(spike_sprite, Spikes[i]->x, Spikes[i]->y, ALIGN_LEFT);

							// Boxes
							for(i = 0; i < nr_boxes; i++)
								drawBitmap(box_sprite, Boxes[i]->x, Boxes[i]->y, ALIGN_LEFT);

							// Special Boxes
							for(i = 0; i < nr_dboxes; i++)
								if(DBox[i]->active)
									drawBitmap(dbox_sprite, DBox[i]->x, DBox[i]->y, ALIGN_LEFT);

							// Platforms
							for(i = 0; i < nr_platforms; i++)
							{
								Platform* p = Platforms[i];
								unsigned int j;
								int size = p->x_size/64;

								for(j=0; j < size; j++)
								{
									int draw_x = p->x + j * 64;
									int draw_y = p->y;
									drawBitmap(platform_sprite, draw_x, draw_y, ALIGN_LEFT);
								}
							}

							// Floor
							for(i = 0; i < getHorResolution(); i++)
							{
								drawBitmap(top_floor_sprite, i*64, 640, ALIGN_LEFT);
								drawBitmap(floor_sprite, i*64, 704, ALIGN_LEFT);
							}

							// Flag
							drawBitmap(flag_sprite, flag->x, flag->y, ALIGN_LEFT);

							// Character
							Bitmap* currentImage = player_animations[myPlayer->anim_position + 5];
							drawBitmap(currentImage, myPlayer->x, myPlayer->y, ALIGN_LEFT);

							// Enemies
							for(i = 0; i < nr_enemies; i++)
								if(Enemies[i]->active)
									drawBitmap(enemy_animations[Enemies[i]->anim_position +5], Enemies[i]->x, Enemies[i]->y, ALIGN_LEFT);

							// Mouse
							if(getMouse()->draw)
								drawFilteredBitmap(pickaxe, getMouse()->x, getMouse()->y, 38655, ALIGN_LEFT);

							break;

						}
						// Controllers
						case 2:
						{
							drawBitmap(ControllersScreen1, 0, 0, ALIGN_LEFT);
							drawBitmap(ControllersScreen2, 512, 0, ALIGN_LEFT);
							drawBitmap(ControllersScreen3, 0, 384, ALIGN_LEFT);
							drawBitmap(ControllersScreen4, 512, 384, ALIGN_LEFT);
							break;
						}
						// High Scores
						case 3:
	 					{
							drawBitmap(HighScoreScreen1, 0, 0, ALIGN_LEFT);
							drawBitmap(HighScoreScreen2, 512, 0, ALIGN_LEFT);
							drawBitmap(HighScoreScreen3, 0, 384, ALIGN_LEFT);
							drawBitmap(HighScoreScreen4, 512, 384, ALIGN_LEFT);

							Score* scores[5];
							for(i = 0; i <= 4; i++)
								*scores[i] = *Scores[i];

							int offsetx;
							int y = 292;

							drawBitmap(nr1, 296, 292, ALIGN_LEFT);
							drawBitmap(nr2, 296, 356, ALIGN_LEFT);
							drawBitmap(nr3, 296, 420, ALIGN_LEFT);
							drawBitmap(nr4, 296, 484, ALIGN_LEFT);
							drawBitmap(nr5, 296, 548, ALIGN_LEFT);

							for(i = 0; i <= 4; i++)
							{
								offsetx = 0;

								if(scores[i]->Score  == 0)
									drawBitmap(nr0, 456, y, ALIGN_LEFT);
								else
								{
									while(scores[i]->Score > 0)
									{
										int digit = scores[i]->Score % 10;
										switch(digit)
										{
										case 0:
										{
											drawBitmap(nr0, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 1:
										{
											drawBitmap(nr1, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 2:
										{
											drawBitmap(nr2, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 3:
										{
											drawBitmap(nr3, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 4:
										{
											drawBitmap(nr4, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 5:
										{
											drawBitmap(nr5, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 6:
										{
											drawBitmap(nr6, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 7:
										{
											drawBitmap(nr7, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 8:
										{
											drawBitmap(nr8, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 9:
										{
											drawBitmap(nr9, 456 - offsetx, y, ALIGN_LEFT);
											break;
										}

										}
										offsetx += 34;
										scores[i]->Score /= 10;
									}
								}

								offsetx = 0;

								while(scores[i]->Day > 0)
								{
									int digit = scores[i]->Day % 10;
									switch(digit)
									{
										case 0:
										{
											drawBitmap(nr0, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 1:
										{
											drawBitmap(nr1, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 2:
										{
											drawBitmap(nr2, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 3:
										{
											drawBitmap(nr3, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 4:
										{
											drawBitmap(nr4, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 5:
										{
											drawBitmap(nr5, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 6:
										{
											drawBitmap(nr6, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 7:
										{
											drawBitmap(nr7, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 8:
										{
											drawBitmap(nr8, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}
										case 9:
										{
											drawBitmap(nr9, 584 - offsetx, y, ALIGN_LEFT);
											break;
										}

										}
									offsetx += 34;
									scores[i]->Day /= 10;
									}
								 drawBitmap(date_sep, 616, y, ALIGN_LEFT);

								offsetx = 0;

								while(scores[i]->Month > 0)
								{
									int digit = scores[i]->Month % 10;
									switch(digit)
									{
									case 0:
									{
										drawBitmap(nr0, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 1:
									{
										drawBitmap(nr1, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 2:
									{
										drawBitmap(nr2, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 3:
									{
										drawBitmap(nr3, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 4:
									{
										drawBitmap(nr4, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 5:
									{
										drawBitmap(nr5, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 6:
									{
										drawBitmap(nr6, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 7:
									{
										drawBitmap(nr7, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 8:
									{
										drawBitmap(nr8, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 9:
									{
										drawBitmap(nr9, 684 - offsetx, y, ALIGN_LEFT);
										break;
									}

									}
									offsetx += 34;
									scores[i]->Month /= 10;
								}
								drawBitmap(date_sep, 718, y, ALIGN_LEFT);

								offsetx = 0;

								while(scores[i]->Year > 0)
								{
									int digit = scores[i]->Year % 10;
									switch(digit)
									{
									case 0:
									{
										drawBitmap(nr0, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 1:
									{
										drawBitmap(nr1, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 2:
									{
										drawBitmap(nr2, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 3:
									{
										drawBitmap(nr3, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 4:
									{
										drawBitmap(nr4, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 5:
									{
										drawBitmap(nr5, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 6:
									{
										drawBitmap(nr6, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 7:
									{
										drawBitmap(nr7, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 8:
									{
										drawBitmap(nr8, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}
									case 9:
									{
										drawBitmap(nr9, 786 - offsetx, y, ALIGN_LEFT);
										break;
									}

								}
								offsetx += 34;
								scores[i]->Year /= 10;
								}

								y += 64;
							}

							break;
						}
						// Credits
						case 4:
						{
							drawBitmap(CreditsScreen1, 0, 0, ALIGN_LEFT);
		 					drawBitmap(CreditsScreen2, 512, 0, ALIGN_LEFT);
							drawBitmap(CreditsScreen3, 0, 384, ALIGN_LEFT);
							drawBitmap(CreditsScreen4, 512, 384, ALIGN_LEFT);
							break;
						}
						// Game Over
						case 5:
						{
							if(myPlayer->hp == 0)
							{
								drawBitmap(GameOverDead1, 0, 0, ALIGN_LEFT);
			 					drawBitmap(GameOverDead2, 512, 0, ALIGN_LEFT);
								drawBitmap(GameOverDead3, 0, 384, ALIGN_LEFT);
								drawBitmap(GameOverDead4, 512, 384, ALIGN_LEFT);
							}

							if(myPlayer->time >= TWO_MINUTES)
							{
								drawBitmap(GameOverTimedOut1, 0, 0, ALIGN_LEFT);
								drawBitmap(GameOverTimedOut2, 512, 0, ALIGN_LEFT);
								drawBitmap(GameOverTimedOut3, 0, 384, ALIGN_LEFT);
								drawBitmap(GameOverTimedOut4, 512, 384, ALIGN_LEFT);
							}
							if ((myPlayer->x + myPlayer->x_size) >= flag->x)
							{
								drawBitmap(GameOver1, 0, 0, ALIGN_LEFT);
								drawBitmap(GameOver2, 512, 0, ALIGN_LEFT);
								drawBitmap(GameOver3, 0, 384, ALIGN_LEFT);
								drawBitmap(GameOver4, 512, 384, ALIGN_LEFT);
							}

							if(myPlayer->score > Scores[4]->Score)
							{
								/* New High Score */

								Score * s = new_score();

								s->Score = myPlayer->score;
								s->Day = rtc->day;
								s->Month = rtc->month;
								s->Year = rtc->year;

								add_score(Scores, s);

								// TODO escrever o score

								myPlayer->score = 0;
							}

							offset = 0;
							int score_draw = score_show;

							if(score_draw  == 0)
								drawBitmap(nr0, 677 - offset, 435, ALIGN_LEFT);
							else
							{
								while(score_draw > 0)
								{
									int digit = score_draw % 10;
									switch(digit)
									{
									case 0:
									{
										drawBitmap(nr0, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 1:
									{
										drawBitmap(nr1, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 2:
									{
										drawBitmap(nr2, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 3:
									{
										drawBitmap(nr3, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 4:
									{
										drawBitmap(nr4, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 5:
									{
										drawBitmap(nr5, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 6:
									{
										drawBitmap(nr6, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 7:
									{
										drawBitmap(nr7, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 8:
									{
										drawBitmap(nr8, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									case 9:
									{
										drawBitmap(nr9, 677 - offset, 435, ALIGN_LEFT);
										break;
									}
									}
									offset += 34;
									score_draw /= 10;
								}
							}
							write_scores(Scores);
						}
						}

			 			// Flips de backbuffer
						bufferToVideoMem();

						// Cleans the buffer
						clear_buffer();
					}

					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		}
	}


	/* Exits Timer */
	if (kbd_unsubscribe_int() != 0)
		return;


	/* Exits Keyboard */
	if (timer_unsubscribe_int() != 0)
		return;

	/* Exits Mouse */
	if(mouse_disable_stream_mode())
		return;

	if (mouse_unsubscribe_int() != 0)
		return;

	mouse_read_outb();

	/* Exits Graphic Mode */
	if(exitVideoGraphics())
	 	return;

	return;
}
