#include "Menu.h"

Menu::Menu()
{
	intro_closed = false;
	new_game = false;
	help = false;
	help_page1 = false;
	help_page2 = false;
	help_page3 = false;

	count_num_of_pause = 0;
	count_breaktime_sound = 0;

	g_sound_game[0] = Mix_LoadWAV("audio//button.wav");
	g_sound_game[1] = Mix_LoadWAV("audio//button2.wav");
	g_sound_game[2] = Mix_LoadWAV("audio//breaktime.wav");
	g_sound_game[3] = Mix_LoadWAV("audio//move_1111.wav");

	pause_hehe.SetColor(Text::WHITE_TEXT);
	score_hehe.SetColor(Text::YELLOW_TEXT);

	str_continue = "Press SPACE button to continue the game";
	str_pause = "Press SPACE button to pause the game";

	Paused_img.setRect(340, 270);

	first_move_left[0] = { 1,1 }; first_move_left[1] = { 8,4 }; first_move_left[2] = { 10,1 }; first_move_left[3] = { 17,4 }; first_move_left[4] = { 1,15 };
	first_move_left[5] = { 6,17 }; first_move_left[6] = { 10,15 }; first_move_left[7] = { 14,19 }; first_move_left[8] = { 17,20 };

	first_move_right[0] = { 10,1 }; first_move_right[1] = { 14,1 }; first_move_right[2] = { 17,1 }; first_move_right[3] = { 10,4 }; first_move_right[4] = { 14,4 };
	first_move_right[5] = { 17,4 }; first_move_right[6] = { 12,4 }; first_move_right[7] = { 14,6 }; first_move_right[8] = { 17,6 };

	first_move_left_duoi[0] = { 1,15 }; first_move_left_duoi[1] = { 4,15 }; first_move_left_duoi[2] = { 6,15 }; first_move_left_duoi[3] = { 8,15 }; first_move_left_duoi[4] = { 8,17 };
	first_move_left_duoi[5] = { 6,17 }; first_move_left_duoi[6] = { 4,17 }; first_move_left_duoi[7] = { 2,19 }; first_move_left_duoi[8] = { 1,20 };

	first_move_right_duoi[0] = { 17,15 }; first_move_right_duoi[1] = { 14,15 }; first_move_right_duoi[2] = { 10,15 }; first_move_right_duoi[3] = { 10,17 }; first_move_right_duoi[4] = { 14,17 };
	first_move_right_duoi[5] = { 14,19 }; first_move_right_duoi[6] = { 17,19 }; first_move_right_duoi[7] = { 12,20 }; first_move_right_duoi[8] = { 17,20 };

	time_to_go_out = 0;
	save_time_when_dead = 0;

	goi_ra_de_chay_vong_lap = 0;

	check_collis_stronger = false;

	score_increase_only_one = 0;
}

Menu::~Menu()
{
	//todo
}

void Menu::CreateIntro(SDL_Event even, BaseObject background, SDL_Renderer* screen)
{
	while (!intro_closed)
	{
	intro_again:
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_KEYDOWN)
			{
				switch (g_event.key.keysym.sym)
				{
				case SDLK_UP:
					background.LoadImg("image//intro//RRR.png", screen);
					new_game = true;
					help = false;
					help_page1 = false;
					help_page2 = false;
					help_page3 = false;
					Mix_PlayChannel(-1, g_sound_game[0], 0);
					break;
				case SDLK_DOWN:
					background.LoadImg("image//intro//RRRR.png", screen);
					new_game = false;
					help = true;
					help_page1 = true;
					Mix_PlayChannel(-1, g_sound_game[0], 0);
					break;
				case SDLK_RETURN:
					Mix_PlayChannel(-1, g_sound_game[1], 0);
					if (new_game)
					{
						intro_closed = true;
						Mix_PlayChannel(1, g_sound_game[3], -1);
					}
					else if (help)
					{
						if (help_page1) {
							background.LoadImg("image//intro//help1.png", screen);
							help_page1 = false;
							help_page2 = true;
						}
						else if (help_page2) {
							background.LoadImg("image//intro//help2.png", screen);
							help_page2 = false;
							help_page3 = true;
						}
						else if (help_page3) {
							background.LoadImg("image//intro//pac_intro.png", screen);
							goto intro_again;
						}
					}
					break;
				default:
					break;
				}
			}
		}

		background.ApplyRender(screen, NULL);
		SDL_RenderPresent(screen);
		SDL_RenderClear(screen);
	}
}

void Menu::HandleIputForPause(SDL_Event even, Pacman& pac, Ghost& blinky, Ghost& pinky, Ghost& inky, Ghost& clyde)
{
	if (even.key.keysym.sym == SDLK_SPACE)
	{
		if (count_num_of_pause % 2 == 0)
		{
			pac.Set_paused(true);
			blinky.Set_paused(true);
			pinky.Set_paused(true);
			inky.Set_paused(true);
			clyde.Set_paused(true);
			count_num_of_pause++;

			if (count_breaktime_sound == 0)
			{
				Mix_PlayChannel(1, g_sound_game[2], -1);
				count_breaktime_sound = 1;
			}
		}
		else
		{
			pac.Set_paused(false);
			blinky.Set_paused(false);
			pinky.Set_paused(false);
			inky.Set_paused(false);
			clyde.Set_paused(false);
			count_num_of_pause++;

			if (count_breaktime_sound == 1)
			{
				Mix_PlayChannel(1, g_sound_game[3], -1);
				count_breaktime_sound = 0;
			}

		}
	}
}

void Menu::ShowPause(TTF_Font* font, SDL_Renderer* screen)
{
	if (count_num_of_pause % 2 == 1)
	{
		Paused_img.LoadImg("map01//pause.png", screen);
		Paused_img.ApplyRender(screen, NULL);

		pause_hehe.SetText(str_continue);

		pause_hehe.LoadFromRenderText(font, screen);
		pause_hehe.RenderText(screen, SCREEN_WIDTH - 520, SCREEN_HEIGHT - 500);
		pause_hehe.Free();

	}
	else
	{
		pause_hehe.SetText(str_pause);

		pause_hehe.LoadFromRenderText(font, screen);
		pause_hehe.RenderText(screen, SCREEN_WIDTH - 520, SCREEN_HEIGHT - 500);
		pause_hehe.Free();
	}
}

void Menu::ShowScore(Pacman pac, Uint32 score_val, TTF_Font* font, SDL_Renderer* screen)
{
	std::string str_score = "Your score: ";

	std::string str_val = std::to_string(score_val);
	str_score += str_val;
	score_hehe.SetText(str_score);

	score_hehe.LoadFromRenderText(font, screen);
	score_hehe.RenderText(screen, SCREEN_WIDTH - 400, SCREEN_HEIGHT - 450);
	score_hehe.Free();
}

bool Menu::CheckCollision(Pacman pac, Ghost ghost)
{
	std::pair<int, int> pac_xy1 = pac.Get_rect();
	std::pair<int, int> ghost_xy1 = ghost.Get_rect();
	std::pair<int, int> pac_xy2; pac_xy2.first = pac_xy1.first + TILE_SIZE; pac_xy2.second = pac_xy1.second;
	std::pair<int, int> ghost_xy2; ghost_xy2.first = ghost_xy1.first + TILE_SIZE; ghost_xy2.second = ghost_xy1.second;
	std::pair<int, int> pac_xy3; pac_xy3.first = pac_xy1.first; pac_xy3.second = pac_xy1.second + TILE_SIZE;
	std::pair<int, int> ghost_xy3; ghost_xy3.first = ghost_xy1.first; ghost_xy3.second = ghost_xy1.second + TILE_SIZE;


	if (pac_xy1.first <= ghost_xy2.first && pac_xy1.first >= ghost_xy2.first - ERROR_NUMBER_OF_GHOST &&
		ghost_xy1.second <= pac_xy2.second + ERROR_NUMBER_OF_GHOST && ghost_xy1.second >= pac_xy2.second - ERROR_NUMBER_OF_GHOST)
	{
		return true;
	}
	else if (pac_xy2.first >= ghost_xy1.first && pac_xy2.first <= ghost_xy1.first + ERROR_NUMBER_OF_GHOST &&
		ghost_xy1.second <= pac_xy2.second + ERROR_NUMBER_OF_GHOST && ghost_xy1.second >= pac_xy2.second - ERROR_NUMBER_OF_GHOST)
	{
		return true;
	}
	else if (ghost_xy3.first >= pac_xy1.first - ERROR_NUMBER_OF_GHOST && ghost_xy3.first <= pac_xy1.first + ERROR_NUMBER_OF_GHOST &&
		pac_xy1.second <= ghost_xy3.second && pac_xy1.second >= ghost_xy3.second - ERROR_NUMBER_OF_GHOST)
	{
		return true;
	}
	else if (ghost_xy1.first >= pac_xy3.first - 5 && ghost_xy1.first <= pac_xy3.first + ERROR_NUMBER_OF_GHOST &&
		pac_xy3.second >= ghost_xy1.second && pac_xy3.second <= ghost_xy1.second + ERROR_NUMBER_OF_GHOST)
	{
		return true;
	}
	return false;
}

bool Menu::DieAndPlayAgain(Pacman& pac, Ghost& blinky, Ghost& pinky, Ghost& inky, Ghost& clyde, GameMap game_map, Map map_1,
	BaseObject background, SDL_Renderer* screen)
{
	bool check = false;//check xem co va cham giua pacman va ghost khong
	if (CheckCollision(pac, blinky) || CheckCollision(pac, pinky) ||
		CheckCollision(pac, inky) || CheckCollision(pac, clyde))
	{
		int count_frame_die = 0;
		while (count_frame_die <= NUM_OF_FRAME_DIE)
		{
			SDL_SetRenderDrawColor(screen, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B, CONTROL_COLOR_TRANSPARENT);
			background.ApplyRender(screen, NULL);
			game_map.DrawMap(screen, map_1);
			pac.ShowDie(screen);
			SDL_Delay(50);
			count_frame_die++;
			SDL_RenderPresent(screen);
			SDL_RenderClear(screen);
		}
		blinky.Set_where_start(370, 240);
		pinky.Set_where_start(340, 300);
		inky.Set_where_start(400, 300);
		clyde.Set_where_start(370, 300);

		pac.Set_original_state();

		check = true;
		save_time_when_dead = SDL_GetTicks();
	}
	return check;
}

bool Menu::BecomeAMonster(Pacman& pac, Ghost& blinky, Ghost& pinky, Ghost& inky, Ghost& clyde, GameMap game_map, Map map_1,
	BaseObject background, SDL_Renderer* screen)
{
	bool check = false;//check xem co va cham giua pacman va ghost khong
	if (goi_ra_de_chay_vong_lap == 0)
	{
		Time_begin_stronger = SDL_GetTicks();
		goi_ra_de_chay_vong_lap = 1;
	}
	if (CheckCollision(pac, blinky) || CheckCollision(pac, pinky) ||
		CheckCollision(pac, inky) || CheckCollision(pac, clyde))
	{
		check_collis_stronger = true;
	}
	if (CheckCollision(pac, blinky))
	{
		if (blinky.get_eat_pacman() == false)
		{
			blinky.Set_alive_status(0);
			Uint32 x = pac.Get_score();
			pac.Set_score(x + 20);
		}
		else 
		{
			DieAndPlayAgain(pac, blinky, pinky, inky, clyde, game_map, map_1, background, screen);
			blinky.set_eat_pacman(true);
		}
	}
	if (CheckCollision(pac, pinky))
	{
		if (pinky.get_eat_pacman() == false)
		{
			pinky.Set_alive_status(0);
			Uint32 x = pac.Get_score();
			pac.Set_score(x + 20);
		}
		else 
		{
			DieAndPlayAgain(pac, blinky, pinky, inky, clyde, game_map, map_1, background, screen);
			pinky.Set_alive_status(3);
			//pinky.set_eat_pacman(true);
		}
	}
	if (CheckCollision(pac, inky))
	{
		if (inky.get_eat_pacman() == false)
		{
			inky.Set_alive_status(0);
			Uint32 x = pac.Get_score();
			pac.Set_score(x + 20);
		}
		else 
		{
			DieAndPlayAgain(pac, blinky, pinky, inky, clyde, game_map, map_1, background, screen);
			inky.set_eat_pacman(true);
		}
	}
	if (CheckCollision(pac, clyde))
	{
		if (clyde.get_eat_pacman() == false)
		{
			clyde.Set_alive_status(0);
			Uint32 x = pac.Get_score();
			pac.Set_score(x + 20);
		}
		else 
		{
			DieAndPlayAgain(pac, blinky, pinky, inky, clyde, game_map, map_1, background, screen);
			clyde.set_eat_pacman(true);
		}
	}
	if (blinky.Get_alive_status() != 0 && blinky.get_eat_pacman() == false)
	{
		blinky.Set_alive_status(1);
	}
	if (pinky.Get_alive_status() != 0 && pinky.get_eat_pacman() == false)
	{
		pinky.Set_alive_status(1);
	}
	if (inky.Get_alive_status() != 0 && inky.get_eat_pacman() == false)
	{
		inky.Set_alive_status(1);
	}
	if (clyde.Get_alive_status() != 0 && clyde.get_eat_pacman() == false)
	{
		clyde.Set_alive_status(1);
	}

	return check;
}

void Menu::GhostMove(Ghost& ghost, int goal_x, int goal_y, Map map_1)
{
	std::pair<int, int> ghost_coor = ghost.Get_current_coordinates_(map_1);
	ghost.set_goal_x(goal_x);
	ghost.set_goal_y(goal_y);

	ghost.BFS(ghost_coor.first, ghost_coor.second, map_1);
	ghost.DoPlayer(map_1);
}

void Menu::FirstProbe(Pacman& pac, Ghost& blinky, Uint32 time_blinky, Ghost& pinky, Uint32 time_pinky, Ghost& inky, Uint32 time_inky,
	Ghost& clyde, Uint32 time_clyde, Map map_1, SDL_Renderer* screen)
{

	time_to_go_out = SDL_GetTicks();
	std::pair<int, int> pac_coor = pac.Get_current_coordinates_(map_1);
	//blinky
	std::pair<int, int> blinky_coor = blinky.Get_current_coordinates_(map_1);
	if (blinky_coor.first == 9 && blinky_coor.second == 9) {
		if (blinky.Get_alive_status() == 0)
		{
			blinky.Set_alive_status(3); pac.set_eat_boss(1, false);
		}
	}
	if (time_to_go_out < 5400 + save_time_when_dead)
	{
		GhostMove(blinky, 17, 1, map_1);
	}
	else if (time_to_go_out < 8300 + save_time_when_dead && time_to_go_out >= 5400 + save_time_when_dead)
	{
		GhostMove(blinky, 10, 4, map_1);
	}
	else if (time_to_go_out < time_blinky + save_time_when_dead && time_to_go_out >= 8300 + save_time_when_dead)
	{
		GhostMove(blinky, 17, 6, map_1);
	}
	else
	{
		if (blinky.Get_alive_status() == 0) {
			GhostMove(blinky, 9, 9, map_1);
		}
		else if (blinky.Get_alive_status() == 1) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 8);

			int index;
			int count = 0;
			if (blinky.get_goal_x() == blinky_coor.first && blinky.get_goal_y() == blinky_coor.second) {
				do {
					index = dis(gen);
					count++;
				} while (first_move_left[index].first == blinky_coor.first && first_move_left[index].second == blinky_coor.second);
			}

			if (count > 0) GhostMove(blinky, first_move_left[index].first, first_move_left[index].second, map_1);
			else GhostMove(blinky, blinky.get_goal_x(), blinky.get_goal_y(), map_1);
		}
		else GhostMove(blinky, pac_coor.first, pac_coor.second, map_1);//blinky di theo muc tieu cua minh o day
	}

	blinky.Show(screen, 0);

	//pinky
	if (time_to_go_out > time_pinky + save_time_when_dead)
	{
		std::pair<int, int> pinky_coor = pinky.Get_current_coordinates_(map_1);
		if (pinky_coor.first == 9 && pinky_coor.second == 9) {
			if (pinky.Get_alive_status() == 0)
			{
				pinky.Set_alive_status(3); pac.set_eat_boss(2, false);
			}
		}
		if (pinky.Get_alive_status() == 0) {
			GhostMove(pinky, 9, 9, map_1);
		}
		else if (pinky.Get_alive_status() == 1) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 8);

			int index;
			int count = 0;
			if (pinky.get_goal_x() == pinky_coor.first && pinky.get_goal_y() == pinky_coor.second) {
				do {
					index = dis(gen);
					count++;
				} while (first_move_left[index].first == pinky_coor.first && first_move_left[index].second == pinky_coor.second);
			}

			if (count > 0) GhostMove(pinky, first_move_left[index].first, first_move_left[index].second, map_1);
			else GhostMove(pinky, pinky.get_goal_x(), pinky.get_goal_y(), map_1);
		}
		else //pinky di thoe muc tieu cua minh o day
		{
			std::pair<int, int> pinky_target = Target_for_pinky(pac, pinky, map_1);
			GhostMove(pinky, pinky_target.first, pinky_target.second, map_1);
		}
	}
	pinky.Show(screen, 1);

	//inky
	if (time_to_go_out > time_inky + save_time_when_dead)
	{
		std::pair<int, int> inky_coor = inky.Get_current_coordinates_(map_1);
		if (inky_coor.first == 9 && inky_coor.second == 9) {
			inky.Set_alive_status(3); pac.set_eat_boss(3, false);
		}
		if (inky.Get_alive_status() == 0) {
			GhostMove(inky, 9, 9, map_1);
		}
		else if (inky.Get_alive_status() == 1) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 8);

			int index;
			int count = 0;
			if (inky.get_goal_x() == inky_coor.first && inky.get_goal_y() == inky_coor.second) {
				do {
					index = dis(gen);
					count++;
				} while (first_move_left[index].first == inky_coor.first && first_move_left[index].second == inky_coor.second);
			}

			if (count > 0) GhostMove(inky, first_move_left[index].first, first_move_left[index].second, map_1);
			else GhostMove(inky, inky.get_goal_x(), inky.get_goal_y(), map_1);
		}
		else //inky di thoe muc tieu cua minh o day
		{
			std::pair<int, int> inky_target = Target_for_inky(pac, blinky, inky, map_1);
			GhostMove(inky, inky_target.first, inky_target.second, map_1);
		}
	}
	inky.Show(screen, 2);

	//clyde
	if (time_to_go_out > time_clyde + save_time_when_dead)
	{
		std::pair<int, int> clyde_coor = clyde.Get_current_coordinates_(map_1);
		if (clyde_coor.first == 9 && clyde_coor.second == 9) {
			clyde.Set_alive_status(3); pac.set_eat_boss(4, false);
		}
		if (clyde.Get_alive_status() == 0) {
			GhostMove(clyde, 9, 9, map_1);
		}
		else if (clyde.Get_alive_status() == 1) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 8);

			int index;
			int count = 0;
			if (clyde.get_goal_x() == clyde_coor.first && clyde.get_goal_y() == clyde_coor.second) {
				do {
					index = dis(gen);
					count++;
				} while (first_move_left[index].first == clyde_coor.first && first_move_left[index].second == clyde_coor.second);
			}

			if (count > 0) GhostMove(clyde, first_move_left[index].first, first_move_left[index].second, map_1);
			else GhostMove(clyde, clyde.get_goal_x(), clyde.get_goal_y(), map_1);
		}
		else //clyde di theo muc tieu cua minh o day
		{
			move_if_the_distance_between_clyde_and_pacman_is_8_cells(pac, clyde, map_1);
		}
	}
	clyde.Show(screen, 3);

}

std::pair<int, int> Menu::Target_for_inky(Pacman pac, Ghost blinky, Ghost inky, Map map_1)
{
	//lay doi xung cua pacman va blinky nen ta se truyen blinky vao ham
	std::pair<int, int> pac_coor = pac.Get_current_coordinates_(map_1);
	std::pair<int, int> blinky_coor = blinky.Get_current_coordinates_(map_1);
	std::pair<int, int> inky_coor = inky.Get_current_coordinates_(map_1);
	std::pair<int, int> target_coor;

	if (inky_coor.first == 8 || inky_coor.first == 9 || inky_coor.first == 10)
	{
		if (inky_coor.second == 10)
		{
			target_coor.first = 9;
			target_coor.second = 8;
			return target_coor;
		}
	}

	target_coor.first = 2 * pac_coor.first - blinky_coor.first;
	target_coor.second = 2 * pac_coor.second - blinky_coor.second;

	if (target_coor.first < 1) target_coor.first = 1;
	if (target_coor.first > 18) target_coor.first = 18;
	if (target_coor.second < 1) target_coor.second = 1;
	if (target_coor.second > 21)target_coor.second = 21;

	else if (map_1.tile[target_coor.first][target_coor.second] == 5 || map_1.tile[target_coor.first][target_coor.second] == 0) return target_coor;
	else return pac_coor;
}

std::pair<int, int> Menu::Target_for_pinky(Pacman pac, Ghost pinky, Map map_1)
{

	//lay vi tri truoc pacman 4 o
	std::pair<int, int> pac_coor = pac.Get_current_coordinates_(map_1);
	std::pair<int, int> pinky_coor = pinky.Get_current_coordinates_(map_1);
	std::pair<int, int> target_coor;

	int n = pac.Get_pacman_status();
	if (n == 2)//left
	{
		target_coor.first = pac_coor.first - 4;
		target_coor.second = pac_coor.second;
	}
	else if (n == 3)//right
	{
		target_coor.first = pac_coor.first + 4;
		target_coor.second = pac_coor.second;
	}
	else if (n == 4)//up
	{
		target_coor.first = pac_coor.first;
		target_coor.second = pac_coor.second - 4;
	}
	else if (n == 5)//down
	{
		target_coor.first = pac_coor.first;
		target_coor.second = pac_coor.second + 4;
	}

	if (target_coor.first < 1) target_coor.first = 1;
	if (target_coor.first > 18) target_coor.first = 18;
	if (target_coor.second < 1) target_coor.second = 1;
	if (target_coor.second > 21)target_coor.second = 21;

	else if (map_1.tile[target_coor.first][target_coor.second] == 5 || map_1.tile[target_coor.first][target_coor.second] == 0) return target_coor;
	else return pac_coor;

	return target_coor;
}

void Menu::move_if_the_distance_between_clyde_and_pacman_is_8_cells(Pacman pac, Ghost& clyde, Map map_1)
{
	std::pair<int, int> pac_coor = pac.Get_current_coordinates_(map_1);
	std::pair<int, int> clyde_coor = clyde.Get_current_coordinates_(map_1);

	float x = sqrt(pow(clyde_coor.first - pac_coor.first, 2) + pow(clyde_coor.second - pac_coor.second, 2));

	if (clyde_coor.first == 8 || clyde_coor.first == 9 || clyde_coor.first == 10)
	{
		if (clyde_coor.second == 10)
		{
			GhostMove(clyde, 8, 9, map_1);
			return;
		}
	}

	if (x < 8)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 8);

		int index;
		int count = 0;
		if (clyde.get_goal_x() == clyde_coor.first && clyde.get_goal_y() == clyde_coor.second) {
			do {
				index = dis(gen);
				count++;
			} while (first_move_left[index].first == clyde_coor.first && first_move_left[index].second == clyde_coor.second);
		}

		if (count > 0) GhostMove(clyde, first_move_left[index].first, first_move_left[index].second, map_1);
		else GhostMove(clyde, clyde.get_goal_x(), clyde.get_goal_y(), map_1);
	}
	else
	{
		GhostMove(clyde, pac_coor.first, pac_coor.second, map_1);
	}
}

