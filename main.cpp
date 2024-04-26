#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "Pacman.h"
#include "Timer.h"
#include "Text.h"
#include "Ghost.h"
#include "Menu.h"

static BaseObject g_background;
TTF_Font* font_score = NULL;
TTF_Font* font_pause = NULL;
TTF_Font* font_continue = NULL;
int MANG = 0;

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

	g_sound_game[4] = Mix_LoadWAV("audio//start.wav");

	return success;
}

bool check_Toado(SDL_Rect Rect, int x, int y) {
	if ((Rect.x < x && x < Rect.x + Rect.w) && (Rect.y < y && y < Rect.y + Rect.h)) return true;
	else return false;
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

int game_over(BaseObject p_over[]) {
	while (SDL_PollEvent(&g_event) != 0)
	{
		if (g_event.type == SDL_QUIT)
		{
			return -1;
		}
		int mouseX = g_event.button.x;
		int mouseY = g_event.button.y;
		if (g_event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (check_Toado(p_over[2].getRect(), mouseX, mouseY) || check_Toado(p_over[2].getRect(), mouseX, mouseY)) {
				return 1;
			}
			if (check_Toado(p_over[4].getRect(), mouseX, mouseY) || check_Toado(p_over[5].getRect(), mouseX, mouseY)) {
				return 0;
			}
		}
		if (check_Toado(p_over[2].getRect(), mouseX, mouseY)) {
			p_over[2].ApplyRender(g_screen);
		}
		else {
			p_over[3].ApplyRender(g_screen);
		}
		if (check_Toado(p_over[4].getRect(), mouseX, mouseY)) {
			p_over[4].ApplyRender(g_screen);
		}
		else {
			p_over[5].ApplyRender(g_screen);
		}
		p_over[0].ApplyRender(g_screen);
		p_over[1].ApplyRender(g_screen);
		SDL_RenderPresent(g_screen);
	}
	SDL_RenderPresent(g_screen);
	return -2;
}

int main(int argc, char* argv[])
{

	if (InitData() == false) return -1;
new_game:
	if (LoadBackground("image//intro//pac_intro.png") == false) return -1;
	g_background.setRect(0, 0);

	//tao bien kieu menu  
	Menu menu;

	//luu score cua nguoi choi
	Uint32 score_val = 0;

	//intro
	menu.CreateIntro(g_event, g_background, g_screen);
	g_background.Free();
	BaseObject p_mang[3];
	for (int i = 0; i < 3; i++) p_mang[i].LoadImg("image//intro//mang.png", g_screen);
	p_mang[0].setRect(850, 300);
	p_mang[1].setRect(900, 300);
	p_mang[2].setRect(950, 300);
new_level:
	//set fps cho game
	Timer game_time;

	//tai map
	GameMap game_map;
	game_map.LoadMap();
	game_map.LoadTiles(g_screen);
	Map map_1 = game_map.GetMap();

	//tai nhan vat pacman
	Pacman p_player;
	p_player.LoadImg("image//pac_img//pacman_right.png", g_screen);
	p_player.SetClips();
	p_player.ArrowImgInit(g_screen);
	if (MANG == 0) p_player.set_mang(3);
	else p_player.set_mang(MANG);
	p_player.Set_score(score_val);

	Ghost blinky, pinky, inky, clyde, shady;

	//blinky
	blinky.LoadImg("image//blinky_img//blinky_up.png", g_screen);
	blinky.SetClip();
	blinky.Set_where_start(370, 240);

	//pinky
	pinky.LoadImg("image//pinky_img//pinky_up.png", g_screen);
	pinky.SetClip();
	pinky.Set_where_start(340, 300);

	//inky
	inky.LoadImg("image//inky_img//inky_up.png", g_screen);
	inky.SetClip();
	inky.Set_where_start(400, 300);

	//clyde
	clyde.LoadImg("image//clyde_img//clyde_up.png", g_screen);
	clyde.SetClip();
	clyde.Set_where_start(370, 300);

	//shady
	shady.LoadImg("image//shady_img//shady_up.png", g_screen);
	shady.SetClip();
	shady.Set_where_start(330, 240);


	//Mix_PlayChannel(1, g_sound_game[4], 0);

	//chay game
	bool is_quit = false;
	int count_blinky_first_move = 0;
	int call_func_blinky = 0;
	Uint32 time_to_go_out;
new_life:
	Mix_Resume(1);//am thanh
	while (!is_quit)
	{
		time_to_go_out = SDL_GetTicks();
		//tinh thoi gian tu thoi diem bat dau
		game_time.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			if (g_event.type == SDL_KEYDOWN)
			{
				//xu li ban phim cho pause
				menu.HandleIputForPause(g_event, p_player, blinky, pinky, inky, clyde);
			}
			//pacman animation
			p_player.HandleInputAction(g_event, g_screen);

		}

		//set mau cho cua so
		SDL_SetRenderDrawColor(g_screen, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B, CONTROL_COLOR_TRANSPARENT);
		g_background.ApplyRender(g_screen, NULL);

		//ve map
		game_map.DrawMap(g_screen, map_1);

		//pacman di chuyen trong map
		std::pair<int, int> pac_coor = p_player.Get_current_coordinates_(map_1);
		p_player.DoPlayer(map_1);
		p_player.Show(g_screen);
		p_player.ShowArrow(g_screen);
		p_player.PacmanMove(map_1);


		//-level-ez---------------------------------------------------------------------------------------------------------------------

		//thoi gian xuat hien cua cac nhan vat
		menu.FirstProbe(p_player, blinky, 10000, pinky, 19000, inky, 25000, clyde, 30000, map_1, g_screen);

		//check xem pacman an trung power dot chua
		bool check_hunter_mode = p_player.Is_hunter_mode();
		if (check_hunter_mode != 0)
		{
			if (p_player.get_eat_boss(1)) {
				blinky.set_eat_pacman(false);
			}
			if (p_player.get_eat_boss(2)) {
				pinky.set_eat_pacman(false);
			}
			if (p_player.get_eat_boss(3)) {
				inky.set_eat_pacman(false);
			}
			if (p_player.get_eat_boss(4)) {
				clyde.set_eat_pacman(false);
			}
			menu.BecomeAMonster_ez(p_player, blinky, pinky, inky, clyde, game_map, map_1, g_background, g_screen);
		}
		else
		{
			//check va cham, pacman mat mang va choi lai mang moi
			blinky.Set_alive_status(3);
			pinky.Set_alive_status(3);
			inky.Set_alive_status(3);
			clyde.Set_alive_status(3);
			bool check_play_again = menu.DieAndPlayAgain_ez(p_player, blinky, pinky, inky, clyde, game_map, map_1, g_background, g_screen);
			if (check_play_again) goto new_life;
		}


		//----------------------------------------------------------------------------------------------------------------------------------------


		if (p_player.Get_num_of_dot() <= 0) {
			MANG = p_player.get_mang();
			goto new_level;
		}
		for (int i = 0; i < p_player.get_mang(); i++) {
			p_mang[i].ApplyRender(g_screen);
		}

		if (p_player.get_mang() <= 0) {
			BaseObject p_over[6];
			p_over[0].LoadImg("image//intro//game_over.png", g_screen); p_over[0].setRect(310, 360);
			p_over[1].LoadImg("image//intro//wanna_try_again.png", g_screen); p_over[1].setRect(705, 200);
			p_over[2].LoadImg("image//intro//yes.png", g_screen); p_over[2].setRect(735, 350);
			p_over[3].LoadImg("image//intro//yes_white.png", g_screen); p_over[3].setRect(735, 350);
			p_over[4].LoadImg("image//intro//no.png", g_screen); p_over[4].setRect(995, 350);
			p_over[5].LoadImg("image//intro//no_white.png", g_screen); p_over[5].setRect(995, 350);
			game_map.DrawMap(g_screen, map_1);
			p_over[3].ApplyRender(g_screen);
			p_over[5].ApplyRender(g_screen);
			p_over[0].ApplyRender(g_screen);
			p_over[1].ApplyRender(g_screen);
			int Cuongg_Over = -2;
			Mix_Pause(1);//am thanh
			do {
				Cuongg_Over = game_over(p_over);
			} while (Cuongg_Over == -2);
			if (Cuongg_Over == 0) is_quit = true;
			else if (Cuongg_Over == 1) {
				MANG = 0;
				score_val = 0;
				goto new_game;
			}
		}

		//tuy chon pause game
		menu.ShowPause(font_continue, g_screen);

		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);

		//show score cua nguoi choi
		score_val = p_player.Get_score();
		menu.ShowScore(p_player, score_val, font_score, g_screen);

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