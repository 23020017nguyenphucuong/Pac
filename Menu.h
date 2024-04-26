#pragma once
#ifndef MENU_H_
#define MENU_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Text.h"

class Menu
{
private:
	bool intro_closed;
	bool new_game;
	bool help;
	bool help_page1;
	bool help_page2;
	bool help_page3;

	int count_num_of_pause;
	int count_breaktime_sound;

	Text pause_hehe;
	Text score_hehe;

	BaseObject Paused_img;

	std::string str_continue;
	std::string str_pause;

	std::pair<int, int> first_move_left[9];
	std::pair<int, int> first_move_right[9];
	std::pair<int, int> first_move_left_duoi[9];
	std::pair<int, int> first_move_right_duoi[9];

	int ran;

	Uint32 time_to_go_out;
	Uint32 save_time_when_dead;
	Uint32 Time_begin_stronger;
	int goi_ra_de_chay_vong_lap;
	bool check_collis_stronger;

public:
	Menu();
	~Menu();
	void CreateIntro(SDL_Event even, BaseObject background, SDL_Renderer* des);
	void HandleIputForPause(SDL_Event even, Pacman& pac, Ghost& ghost1, Ghost& ghost2, Ghost& ghost3, Ghost& ghost4);

	void ShowPause(TTF_Font* font, SDL_Renderer* screen);
	void ShowScore(Pacman pac, Uint32 score_val, TTF_Font* font, SDL_Renderer* screen);

	bool CheckCollision(Pacman pac, Ghost ghost);

	bool DieAndPlayAgain_ez(Pacman& pac, Ghost& blinky, Ghost& pinky, Ghost& inky, Ghost& clyde, GameMap game_map, Map map_1,
		BaseObject background, SDL_Renderer* screen);

	bool BecomeAMonster_ez(Pacman& pac, Ghost& blinky, Ghost& pinky, Ghost& inky, Ghost& clyde, GameMap game_map, Map map_1,
		BaseObject background, SDL_Renderer* screen);

	void FirstProbe(Pacman& pac, Ghost& blinky, Uint32 time_blinky, Ghost& pinky, Uint32 time_pinky,
		Ghost& inky, Uint32 time_inky, Ghost& clyde, Uint32 time_clyde, Map map_1, SDL_Renderer* screen);

	void ShadyFirstProbe(Pacman& pac, Ghost& shady, Uint32 time_shady, Map map_1, SDL_Renderer* screen);

	void GhostMove(Ghost& ghost, int goal_x, int goal_y, Map map_1);

	void Set_time_to_go_out(Uint32 x) { time_to_go_out = x; }

	void Set_save_time_when_dead(Uint32 x) { save_time_when_dead = x; }

	void move_if_the_distance_between_clyde_and_pacman_is_8_cells(Pacman pac, Ghost& clyde, Map map_1);

	std::pair<int, int> Target_for_inky(Pacman pac, Ghost blinky, Ghost inky, Map map_1);

	std::pair<int, int> Target_for_pinky(Pacman pac, Ghost pinky, Map map_1);
};

#endif // !MENU_H_
