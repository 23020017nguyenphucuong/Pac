#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "Pacman.h"
#include "Timer.h"
#include "Text.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

static BaseObject g_background;
TTF_Font* font_score = NULL;
TTF_Font* font_pause = NULL;
TTF_Font* font_continue = NULL;


//khai bao chuong trinh va in background
bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("PacMan Demo by NPC",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (g_window == NULL) success = false;
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B, CONTROL_COLOR_TRANSPARENT);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
		}

		if (TTF_Init() == -1)
		{
			success = false;
		}

		font_score = TTF_OpenFont("font//FFFFORWA.TTF", 25);
		font_pause = TTF_OpenFont("font//FFFFORWA.TTF", 20);
		font_continue = TTF_OpenFont("font//FFFFORWA.TTF", 19);
		if (font_score == NULL || font_pause == NULL)
		{
			success = false;
		}
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		success = false;
	}

	g_sound_game[0] = Mix_LoadWAV("audio//button.wav");
	g_sound_game[1] = Mix_LoadWAV("audio//button2.wav");
	g_sound_game[2] = Mix_LoadWAV("audio//breaktime.wav");
	g_sound_game[3] = Mix_LoadWAV("audio//move_1111.wav");
	g_sound_game[4] = Mix_LoadWAV("audio//start.wav");

	return success;
}

bool LoadBackground(std::string path)
{
	bool ret = g_background.LoadImg(path, g_screen);
	if (ret == false) return false;
	return true;
}

void Close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (InitData() == false) return -1;
	if (LoadBackground("image//intro//pac_intro.png") == false) return -1;
    g_background.setRect(0, 0);

    //tai map
	GameMap game_map;
	game_map.LoadMap();
	game_map.LoadTiles(g_screen);
	Map map_1 = game_map.GetMap();

    //tai nhan vat pacman
	static Pacman p_player;
	p_player.LoadImg("image//pac_img//pacman_right.png", g_screen);
	p_player.SetClips();
	p_player.ArrowImgInit(g_screen);

    //set fps cho game
	Timer game_time;

	//score cua nguoi choi
	Text score_hehe;
    score_hehe.SetColor(Text::YELLOW_TEXT);

	//in len man hinh huong dan continue and pause game
	Text pause_hehe;
	pause_hehe.SetColor(Text::WHITE_TEXT);

	Text continue_hehe;
	continue_hehe.SetColor(Text::YELLOW_TEXT);

	BaseObject Paused_img;
	Paused_img.LoadImg("map01//pause.png", g_screen);
	Paused_img.setRect(340, 270);

	//blinky
	Blinky blinky;
	blinky.LoadImg("image//blinky_img//blinky_up.png", g_screen);
	blinky.SetClip();

	//pinky
	Pinky pinky;
	pinky.LoadImg("image//pinky_img//pinky_up.png", g_screen);
	pinky.SetClip();

	//inky
	Inky inky;
	inky.LoadImg("image//inky_img//inky_up.png", g_screen);
	inky.SetClip();

	//pinky
	Clyde clyde;
	clyde.LoadImg("image//clyde_img//clyde_up.png", g_screen);
	clyde.SetClip();
	
    //intro
	bool intro_closed = false;
	bool new_game = false;
	bool help = false;
	SDL_Texture* intro_;
	while (!intro_closed)
	{
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_KEYDOWN)
			{
				switch (g_event.key.keysym.sym)
				{
				case SDLK_UP:
					LoadBackground("image//intro//RRR.png");
					new_game = true;
					help = false;
					Mix_PlayChannel(-1, g_sound_game[0], 0);
					break;
				case SDLK_DOWN:
					LoadBackground("image//intro//RRRR.png");
					new_game = false;
					help = true;
					Mix_PlayChannel(-1, g_sound_game[0], 0);
					break;
				case SDLK_RETURN:
					if (new_game == true) intro_closed = true;
					//else if(help = true) todo;
					Mix_PlayChannel(-1, g_sound_game[1], 0);
					break;
				default:
					break;
				}
			}
		}
		
		g_background.ApplyRender(g_screen, NULL);
		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);
	}

	//Mix_PlayChannel(1, g_sound_game[4], 0);
	Mix_PlayChannel(1, g_sound_game[3], -1);

	//chay game
	int count_num_of_pause = 0;
	bool is_quit = false;
	int count_intro_sound = 0;
	while (!is_quit)
	{

		game_time.start();//tinh thoi gian tu thoi diem bat dau
		while (SDL_PollEvent(&g_event) != 0)
		{
            
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			if (g_event.type == SDL_KEYDOWN)
			{
				//set cac nhan vat o che do tam dung
				if (g_event.key.keysym.sym == SDLK_SPACE)
				{
					if (count_num_of_pause % 2 == 0)
					{
						p_player.Set_paused(true);
						//pinky.Set_paused(true);
						count_num_of_pause++;

						if (count_intro_sound == 0)
						{
							Mix_PlayChannel(1, g_sound_game[2], -1);
							count_intro_sound = 1;
						}
					}
					else
					{
						p_player.Set_paused(false);
						//pinky.Set_paused(false);
						count_num_of_pause++;
                       
						if (count_intro_sound == 1)
						{
							Mix_PlayChannel(1, g_sound_game[3], -1);
							count_intro_sound = 0;
						}

					}
				}
			}
			p_player.HandleInputAction(g_event, g_screen);//pacman animation

		}

        //set mau cho cua so
		SDL_SetRenderDrawColor(g_screen, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B, CONTROL_COLOR_TRANSPARENT);
		//g_background.ApplyRender(g_screen, NULL);

        //ve map
		game_map.DrawMap(g_screen,map_1);

        //pacman di chuyen trong map
		p_player.DoPlayer(map_1);
		p_player.Show(g_screen);
		p_player.ShowArrow(g_screen);
		p_player.PacmanMove(map_1);

		//blinky
		blinky.DoPlayer(map_1);
		blinky.Show(g_screen);
		std::pair<int, int> pac_coor = p_player.Get_current_coordinates_(map_1);
		std::pair<int, int> blinky_coor = blinky.Get_current_coordinates_(map_1);
        blinky.BlinkyMove(map_1);
		blinky.BlinkyMove1(map_1,blinky_coor,pac_coor);
		

		//pinky
		pinky.DoPlayer(map_1);
		pinky.Show(g_screen);
		//pinky.PinkyMove(map_1);
		pinky.PinkyMove1(map_1);

		//inky
		std::pair<int, int> inky_coor = inky.Get_current_coordinates_();
		
		
		inky.set_goal_x(pac_coor.first);
		inky.set_goal_y(pac_coor.second);
		
		inky.Find(inky_coor.first, inky_coor.second,map_1);
		inky.DoPlayer(map_1);
		
		inky.Show(g_screen);
		//clyde
		clyde.DoPlayer(map_1);
		clyde.Show(g_screen);
		clyde.ClydeMove1();

		//show anh dung game
		if (count_num_of_pause % 2 == 1)
		{
			Paused_img.ApplyRender(g_screen, NULL);
		}

		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);

		//show score cua nguoi choi
		std::string str_score = "Your score: ";
		Uint32 score_val = p_player.Get_score();

		std::string str_val = std::to_string(score_val);
		str_score += str_val;
		score_hehe.SetText(str_score);

		score_hehe.LoadFromRenderText(font_score, g_screen);
		score_hehe.RenderText(g_screen, SCREEN_WIDTH - 400, SCREEN_HEIGHT - 450);
		score_hehe.Free();

        //show huong dan continue va pause game
		if (count_num_of_pause % 2 == 1)
		{
			std::string str_continue = "Press SPACE button to continue the game";

			pause_hehe.SetText(str_continue);

			pause_hehe.LoadFromRenderText(font_continue, g_screen);
			pause_hehe.RenderText(g_screen, SCREEN_WIDTH - 520, SCREEN_HEIGHT - 500);
			pause_hehe.Free();

		}
		else
		{
			std::string str_pause = "Press SPACE button to pause the game";

			pause_hehe.SetText(str_pause);

			pause_hehe.LoadFromRenderText(font_continue, g_screen);
			pause_hehe.RenderText(g_screen, SCREEN_WIDTH - 520, SCREEN_HEIGHT - 500);
			pause_hehe.Free();
		}

        //dieu chinh fps cua game
		int real_time = game_time.get_sticks();
		int time_one_frame = 1000 / FPS;
		if (real_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}
	}

	Close();
	return 0;
}