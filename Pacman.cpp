#include "Pacman.h"

Pacman::Pacman()
{
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 370;
	y_pos_ = 360;
	width_frame_ = 30;
	height_frame_ = 30;
	frame_ = 0;
	frame_die_ = 0;

	pacman_status_ = 0;
	arrow_status_ = 0;
	alive_status_ = ALIVE;

	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;

	on_wall = false;
	score = 0;
	number_of_dot = 184;
	paused_ = false;

	eat_dot_sound = 0;

	g_sound_pac[0] = Mix_LoadWAV("audio//eat_dot_1.wav");
	eat_boss1 = false;
	eat_boss2 = false;
	eat_boss3 = false;
	eat_boss4 = false;
	eat_boss5 = false;
	start_time_strong = 0;
	end_time_strong = 0;
	mang = 3;
}

Pacman::~Pacman()
{
	//todo
}

bool Pacman::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w / NUM_OF_FRAME_MOVE;
		height_frame_ = rect_.h;
	}
	return ret;
}

void Pacman::SetClips()
{
	if (width_frame_ > 0 && height_frame_ > 0) //xac dinh trang thai cua tung frame
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		for (int i = 1; i < NUM_OF_FRAME_MOVE; i++)
		{
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}

		frame_clip_die_[0].x = 0;
		frame_clip_die_[0].y = 0;
		frame_clip_die_[0].w = width_frame_;
		frame_clip_die_[0].h = height_frame_;

		for (int i = 1; i < NUM_OF_FRAME_DIE; i++)
		{
			frame_clip_die_[i].x = i * width_frame_;
			frame_clip_die_[i].y = 0;
			frame_clip_die_[i].w = width_frame_;
			frame_clip_die_[i].h = height_frame_;
		}
	}

}

void Pacman::Show(SDL_Renderer* des)
{
	//tai anh theo huong di chuyen
	if (pacman_status_ == WALK_LEFT)
	{
		LoadImg("image//pac_img//pacman_left.png", des);
	}
	if (pacman_status_ == WALK_RIGHT)
	{
		LoadImg("image//pac_img//pacman_right.png", des);
	}
	if (pacman_status_ == WALK_UP)
	{
		LoadImg("image//pac_img//pacman_up.png", des);
	}
	if (pacman_status_ == WALK_DOWN)
	{
		LoadImg("image//pac_img//pacman_down.png", des);
	}

	if (paused_ == false)
	{
		if (input_type_.left_ == 1 || //doi frame sau moi lan bam 
			input_type_.right_ == 1 ||
			input_type_.up_ == 1 ||
			input_type_.down_ == 1)
		{
			frame_++;
		}
		if (frame_ >= NUM_OF_FRAME_MOVE)
		{
			frame_ = 0;
		}
	}

	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_[frame_];//frame clip hien tai
	SDL_Rect renderQuad = { rect_.x,rect_.y,width_frame_,height_frame_ };//kich thuoc chuan nhat
	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void Pacman::ShowDie(SDL_Renderer* des)
{
	LoadImg("image//pac_img//pacman_die.png", des);

	if (paused_ == false)
	{
		frame_die_++;

		if (frame_die_ >= NUM_OF_FRAME_DIE)
		{
			frame_die_ = 0;
		}
	}

	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_die_[frame_die_];//frame clip hien tai
	SDL_Rect renderQuad = { rect_.x,rect_.y,30,height_frame_ };//kich thuoc chuan nhat
	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

void Pacman::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	//xu li ban phim
	if (paused_ == false)
	{
		if (events.type == SDL_KEYDOWN)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_LEFT:
				arrow_status_ = WALK_LEFT;
				break;
			case SDLK_RIGHT:
				arrow_status_ = WALK_RIGHT;
				break;
			case SDLK_UP:
				arrow_status_ = WALK_UP;
				break;
			case SDLK_DOWN:
				arrow_status_ = WALK_DOWN;
				break;
			default:
				break;
			}
		}
	}
}

void Pacman::ArrowImgInit(SDL_Renderer* des)
{
	arrow_direct[0].LoadImg("image//pac_img//arrow_left.png", des);
	arrow_direct[1].LoadImg("image//pac_img//arrow_right.png", des);
	arrow_direct[2].LoadImg("image//pac_img//arrow_up.png", des);
	arrow_direct[3].LoadImg("image//pac_img//arrow_down.png", des);
}

void Pacman::ShowArrow(SDL_Renderer* des)
{
	if (arrow_status_ == WALK_LEFT)
	{
		SDL_Rect renderQuad = { rect_.x - 9, rect_.y + 7, 7, 16 };//kich thuoc chuan nhat

		SDL_RenderCopy(des, arrow_direct[0].getObject(), NULL, &renderQuad);

	}
	if (arrow_status_ == WALK_RIGHT)
	{
		SDL_Rect renderQuad = { rect_.x + 32, rect_.y + 7, 7, 16 };//kich thuoc chuan nhat

		SDL_RenderCopy(des, arrow_direct[1].getObject(), NULL, &renderQuad);
	}
	if (arrow_status_ == WALK_UP)
	{
		SDL_Rect renderQuad = { rect_.x + 7, rect_.y - 9, 16, 7 };//kich thuoc chuan nhat

		SDL_RenderCopy(des, arrow_direct[2].getObject(), NULL, &renderQuad);
	}
	if (arrow_status_ == WALK_DOWN)
	{
		SDL_Rect renderQuad = { rect_.x + 7, rect_.y + 32, 16, 7 };//kich thuoc chuan nhat

		SDL_RenderCopy(des, arrow_direct[3].getObject(), NULL, &renderQuad);
	}
}

void Pacman::DoPlayer(Map& map_data)
{
	x_val_ = 0;
	y_val_ = 0;

	if (paused_ == false)
	{
		if (input_type_.left_ == 1)
		{
			x_val_ -= PACMAN_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PACMAN_SPEED;
		}
		else if (input_type_.up_ == 1)
		{
			y_val_ -= PACMAN_SPEED;
		}
		else if (input_type_.down_ == 1)
		{
			y_val_ += PACMAN_SPEED;
		}
		CheckToMap(map_data);
	}

}

void Pacman::CheckToMap(Map& map_data)
{
	start_time_strong = SDL_GetTicks();
	int x1 = 0;//gioi han kiem tra tu a den b theo chieu x
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;
	//check theo chieu ngang truoc

	x1 = (x_pos_ - SIDE_LEFT + x_val_) / TILE_SIZE;//o thu bao nhieu
	x2 = (x_pos_ - SIDE_LEFT + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	if (x_val_ > 0)//sang phai
	{
		if (map_data.tile[y1][x2] == DOT_TILE || map_data.tile[y2][x2] == DOT_TILE)
		{
			map_data.tile[y1][x2] = BLANK_TILE;
			map_data.tile[y2][x2] = BLANK_TILE;

			number_of_dot--;
			score += 10;

			if (eat_dot_sound == 0) Mix_PlayChannel(-1, g_sound_pac[0], 0);
			eat_dot_sound++;
			if (eat_dot_sound == 2) eat_dot_sound = 0;
		}
		else if (map_data.tile[y1][x2] == HUNTER_MODE_TILE || map_data.tile[y2][x2] == HUNTER_MODE_TILE)
		{
			map_data.tile[y1][x2] = BLANK_TILE;
			map_data.tile[y2][x2] = BLANK_TILE;

			number_of_dot--;
			score += 20;

			hunter_mode++; eat_boss1 = true; eat_boss2 = true; eat_boss3 = true; eat_boss4 = true; eat_boss5 = true;
			end_time_strong = start_time_strong + 10000;
		}
		else
		{
			if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
			{
				if (map_data.tile[y1][x2] == GALAXY_RIGHT_TILE || map_data.tile[y2][x2] == GALAXY_RIGHT_TILE)
				{
					x_pos_ = SIDE_LEFT + 10;
					x_val_ = 0;
				}
				else
				{
					x_pos_ = x2 * TILE_SIZE - width_frame_ + SIDE_LEFT;
					x_val_ = 0;
					input_type_.right_ = 0;
					on_wall = true;
					int i = 0; int j = 0;
				}
			}
		}

	}
	else if (x_val_ < 0)//sang trai
	{
		if (map_data.tile[y1][x1] == DOT_TILE || map_data.tile[y2][x1] == DOT_TILE)
		{
			map_data.tile[y1][x1] = BLANK_TILE;
			map_data.tile[y2][x1] = BLANK_TILE;

			number_of_dot--;
			score += 10;

			if (eat_dot_sound == 0) Mix_PlayChannel(-1, g_sound_pac[0], 0);
			eat_dot_sound++;
			if (eat_dot_sound == 2) eat_dot_sound = 0;
		}
		else if (map_data.tile[y1][x1] == HUNTER_MODE_TILE || map_data.tile[y2][x1] == HUNTER_MODE_TILE)
		{
			map_data.tile[y1][x1] = BLANK_TILE;
			map_data.tile[y2][x1] = BLANK_TILE;

			number_of_dot--;
			score += 20;

			hunter_mode++; eat_boss1 = true; eat_boss2 = true; eat_boss3 = true; eat_boss4 = true; eat_boss5 = true;
			end_time_strong = start_time_strong + 10000;
		}
		else
		{
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
			{
				if (map_data.tile[y1][x1] == GALAXY_LEFT_TILE || map_data.tile[y2][x1] == GALAXY_LEFT_TILE)
				{
					x_pos_ = SIDE_LEFT + 18 * TILE_SIZE - 10;
					x_val_ = 0;
				}
				else
				{
					x_pos_ = (x1 + 1) * TILE_SIZE + SIDE_LEFT;
					x_val_ = 0;
					input_type_.left_ = 0;
					on_wall = true;
				}
			}
		}
	}
	else if (y_val_ > 0)//xuong duoi
	{
		if (map_data.tile[y2][x1] == DOT_TILE || map_data.tile[y2][x2] == DOT_TILE)
		{
			map_data.tile[y2][x1] = BLANK_TILE;
			map_data.tile[y2][x2] = BLANK_TILE;

			number_of_dot--;
			score += 10;

			if (eat_dot_sound == 0) Mix_PlayChannel(-1, g_sound_pac[0], 0);
			eat_dot_sound++;
			if (eat_dot_sound == 2) eat_dot_sound = 0;
		}
		else if (map_data.tile[y2][x1] == HUNTER_MODE_TILE || map_data.tile[y2][x2] == HUNTER_MODE_TILE)
		{
			map_data.tile[y2][x1] = BLANK_TILE;
			map_data.tile[y2][x2] = BLANK_TILE;

			number_of_dot--;
			score += 20;

			hunter_mode++; eat_boss1 = true; eat_boss2 = true; eat_boss3 = true; eat_boss4 = true; eat_boss5 = true;
			end_time_strong = start_time_strong + 10000;
		}
		else
		{
			if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
			{
				y_pos_ = y2 * TILE_SIZE - height_frame_;
				y_val_ = 0;
				input_type_.down_ = 0;
				on_wall = true;
			}
		}
	}
	else if (y_val_ < 0)//len tren
	{
		if (map_data.tile[y1][x1] == DOT_TILE || map_data.tile[y1][x2] == DOT_TILE)
		{
			map_data.tile[y1][x1] = BLANK_TILE;
			map_data.tile[y1][x2] = BLANK_TILE;

			number_of_dot--;
			score += 10;

			if (eat_dot_sound == 0) Mix_PlayChannel(-1, g_sound_pac[0], 0);
			eat_dot_sound++;
			if (eat_dot_sound == 2) eat_dot_sound = 0;
		}
		else if (map_data.tile[y1][x1] == HUNTER_MODE_TILE || map_data.tile[y1][x2] == HUNTER_MODE_TILE)
		{
			map_data.tile[y1][x1] = BLANK_TILE;
			map_data.tile[y1][x2] = BLANK_TILE;

			number_of_dot--;
			score += 20;

			hunter_mode++; eat_boss1 = true; eat_boss2 = true; eat_boss3 = true; eat_boss4 = true; eat_boss5 = true;
			end_time_strong = start_time_strong + 10000;
		}
		else
		{
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
				input_type_.up_ = 0;
				on_wall = true;
			}
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;
	if (start_time_strong >= end_time_strong) {
		hunter_mode = 0;
		eat_boss1 = false; eat_boss2 = false; eat_boss3 = false; eat_boss4 = false; eat_boss5 = false;
	}

}

void Pacman::PacmanMove(Map map_data)
{
	int x1 = 0;//gioi han kiem tra tu a den b theo chieu x
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//check theo chieu ngang truoc

	x1 = (x_pos_ - SIDE_LEFT + x_val_) / TILE_SIZE;//o thu bao nhieu
	x2 = (x_pos_ - SIDE_LEFT + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;
	if (on_wall == false) {
		if (arrow_status_ == WALK_RIGHT && pacman_status_ != WALK_RIGHT)
		{
			if (map_data.tile[y1][x2 + 1] == BLANK_TILE && map_data.tile[y2][x2 + 1] == BLANK_TILE ||
				map_data.tile[y1][x2 + 1] == DOT_TILE && map_data.tile[y2][x2 + 1] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
				if (pacman_status_ == WALK_UP) y_pos_ -= PACMAN_SPEED;
				else if (pacman_status_ == WALK_DOWN) y_pos_ += PACMAN_SPEED;
				pacman_status_ = WALK_RIGHT;

			}
		}
		else if (arrow_status_ == WALK_LEFT && pacman_status_ != WALK_LEFT)
		{
			if (map_data.tile[y1][x1 - 1] == BLANK_TILE && map_data.tile[y2][x1 - 1] == BLANK_TILE ||
				map_data.tile[y1][x1 - 1] == DOT_TILE && map_data.tile[y2][x1 - 1] == DOT_TILE)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
				if (pacman_status_ == WALK_UP) y_pos_ -= PACMAN_SPEED;
				else if (pacman_status_ == WALK_DOWN) y_pos_ += PACMAN_SPEED;
				pacman_status_ = WALK_LEFT;
			}

		}
		else if (arrow_status_ == WALK_UP && pacman_status_ != WALK_UP)
		{
			if (map_data.tile[y1 - 1][x1] == BLANK_TILE && map_data.tile[y1 - 1][x2] == BLANK_TILE ||
				map_data.tile[y1 - 1][x1] == DOT_TILE && map_data.tile[y1 - 1][x2] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 0;
				input_type_.up_ = 1;
				input_type_.down_ = 0;
				if (pacman_status_ == WALK_RIGHT) {
					x_pos_ = ((x_pos_ - 300) / TILE_SIZE + 1) * TILE_SIZE + 300 - (TILE_SIZE - PACMAN_SPEED);
				}
				else if (pacman_status_ == WALK_LEFT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE - 1) * TILE_SIZE + 300 + (TILE_SIZE - PACMAN_SPEED);
				pacman_status_ = WALK_UP;
			}

		}
		else if (arrow_status_ == WALK_DOWN && pacman_status_ != WALK_DOWN)
		{
			if (map_data.tile[y2 + 1][x1] == BLANK_TILE && map_data.tile[y2 + 1][x2] == BLANK_TILE ||
				map_data.tile[y2 + 1][x1] == DOT_TILE && map_data.tile[y2 + 1][x2] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 1;
				if (pacman_status_ == WALK_RIGHT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE + 1) * TILE_SIZE + 300 - (TILE_SIZE - PACMAN_SPEED);
				else if (pacman_status_ == WALK_LEFT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE - 1) * TILE_SIZE + 300 + (TILE_SIZE - PACMAN_SPEED);
				pacman_status_ = WALK_DOWN;
			}
		}
	}
	else {
		if (arrow_status_ == WALK_LEFT) {
			input_type_.left_ = 1;
			pacman_status_ = WALK_LEFT;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_RIGHT) {
			input_type_.right_ = 1;
			pacman_status_ = WALK_RIGHT;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_UP) {
			input_type_.up_ = 1;
			pacman_status_ = WALK_UP;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_DOWN) {
			input_type_.down_ = 1;
			pacman_status_ = WALK_DOWN;
			on_wall = false;
		}
	}
}

std::pair<int, int> Pacman::Get_current_coordinates_(Map map_data)
{
	int x1 = (x_pos_ + x_val_ - SIDE_LEFT + 5) / TILE_SIZE;//o thu bao nhieu
	int y1 = (y_pos_ + y_val_ + 5) / TILE_SIZE;
	std::pair<int, int> coor = { x1,y1 };
	return coor;
}

void Pacman::Set_original_state()
{
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 370;
	y_pos_ = 360;
	width_frame_ = 30;
	height_frame_ = 30;
	frame_ = 0;
	frame_die_ = 0;

	pacman_status_ = 0;
	arrow_status_ = 0;
	alive_status_ = ALIVE;

	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;

	on_wall = false;
	paused_ = false;
	reduce_mang();
}



