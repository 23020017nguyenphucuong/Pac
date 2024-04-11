#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "Pacman.h"
#include "Timer.h"

static BaseObject g_background;

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
	}
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


	GameMap game_map;//tai map
	game_map.LoadMap();
	game_map.LoadTiles(g_screen);

	Pacman p_player;//tai nhan vat pacman
	p_player.LoadImg("image//pac_img//pacman_right.png", g_screen);
	p_player.SetClips();
	p_player.ArrowImgInit(g_screen);

	Timer game_time;//set fps cho game
	
    
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
					break;
				case SDLK_DOWN:
					LoadBackground("image//intro//RRRR.png");
					new_game = false;
					help = true;
					break;
				case SDLK_RETURN:
					if (new_game == true) intro_closed = true;
					//else if(help = true) todo;
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

	bool is_quit = false;
	while (!is_quit)
	{
		game_time.start();//tinh thoi gian tu thoi diem bat dau
		while (SDL_PollEvent(&g_event) != 0)
		{
            
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			p_player.HandleInputAction(g_event, g_screen);//pacman animation

		}

		SDL_SetRenderDrawColor(g_screen, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B, CONTROL_COLOR_TRANSPARENT);//set mau cho cua so
		//g_background.ApplyRender(g_screen, NULL);

		game_map.DrawMap(g_screen);//map
		Map map_1 = game_map.GetMap();

		p_player.DoPlayer(map_1);//pacman di chuyen trong map
		p_player.Show(g_screen);
		p_player.ShowArrow(g_screen);
		
		

		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);

		int real_time = game_time.get_sticks();//dieu chinh fps cua game
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