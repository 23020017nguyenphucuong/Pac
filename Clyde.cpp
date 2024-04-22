#include "Clyde.h"
#include <SDL_timer.h>

Clyde::Clyde()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 280;//toa do
	y_pos_ = 300;
	frame_ = 0;
	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	paused_ = false;
	lastUpdateTime = 0;
}

Clyde::~Clyde()
{
	//todo
}

void Clyde::SetClip()
{
	if (width_frame_ > 0 && height_frame_ > 0) //xac dinh trang thai cua tung frame
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		for (int i = 1; i < NUM_OF_GHOST_FRAME; i++)
		{
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

bool Clyde::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / NUM_OF_GHOST_FRAME;
		height_frame_ = rect_.h;
	}
	return false;
}

void Clyde::Show(SDL_Renderer* des)
{
	if (input_type_.left_ == 1)
	{
		LoadImg("image//clyde_img//clyde_left.png", des);
	}
	else if (input_type_.right_ == 1)
	{
		LoadImg("image//clyde_img//clyde_right.png", des);
	}
	else if (input_type_.up_ == 1)
	{
		LoadImg("image//clyde_img//clyde_up.png", des);
	}
	else if (input_type_.down_ == 1)
	{
		LoadImg("image//clyde_img//clyde_down.png", des);
	}
	if (paused_ == false)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		frame_++;
		if (frame_ >= NUM_OF_GHOST_FRAME)
		{
			frame_ = 0;
		}
	}

	SDL_Rect* current_clip = &frame_clip_[frame_];//frame clip hien tai
	SDL_Rect renderQuad = { rect_.x,rect_.y,width_frame_,height_frame_ };//kich thuoc chuan nhat
	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void Clyde::DoPlayer(Map& map_data)
{
	x_val_ = 0;
	y_val_ = 0;

	if (paused_ == false)
	{
		if (input_type_.left_ == 1)
		{
			x_val_ -= GHOST_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += GHOST_SPEED;
		}
		else if (input_type_.up_ == 1)
		{
			y_val_ -= GHOST_SPEED;
		}
		else if (input_type_.down_ == 1)
		{
			y_val_ += GHOST_SPEED;
		}
		CheckToMap(map_data);
	}
}

void Clyde::ClydeMove1()
{
	Uint32 currentTime = SDL_GetTicks();

	if (currentTime - lastUpdateTime >= 1000)
	{
		lastUpdateTime = currentTime;

		int randomDirection = rand() % 4;

		switch (randomDirection)
		{
		case 0:
			arrow_status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			break;
		case 1:
			arrow_status_ = WALK_RIGHT;
			input_type_.left_ = 0;
			input_type_.right_ = 1;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			break;
		case 2:
			arrow_status_ = WALK_UP;
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 1;
			input_type_.down_ = 0;
			break;
		case 3:
			arrow_status_ = WALK_DOWN;
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 1;
			break;
		default:
			break;
		}
	}
}

void Clyde::CheckToMap(Map& map_data)
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

	if (x_val_ > 0)//sang phai
	{
		if (map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != DOT_TILE && map_data.tile[y1][x2] != HUNTER_MODE_TILE && map_data.tile[y1][x2] != GHOST_DOOR_TILE ||
			map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != DOT_TILE && map_data.tile[y2][x2] != HUNTER_MODE_TILE && map_data.tile[y2][x2] != GHOST_DOOR_TILE)
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
			}
		}

	}
	else if (x_val_ < 0)//sang trai
	{
		if (map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != DOT_TILE && map_data.tile[y1][x1] != HUNTER_MODE_TILE && map_data.tile[y1][x1] != GHOST_DOOR_TILE ||
			map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != DOT_TILE && map_data.tile[y2][x1] != HUNTER_MODE_TILE && map_data.tile[y2][x1] != GHOST_DOOR_TILE)
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
			}
		}
	}
	else if (y_val_ > 0)//xuong duoi
	{
		if (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != DOT_TILE && map_data.tile[y2][x1] != HUNTER_MODE_TILE && map_data.tile[y2][x1] != GHOST_DOOR_TILE ||
			map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != DOT_TILE && map_data.tile[y2][x2] != HUNTER_MODE_TILE && map_data.tile[y2][x2] != GHOST_DOOR_TILE)
		{
			y_pos_ = y2 * TILE_SIZE - height_frame_;
			y_val_ = 0;
		}
	}
	else if (y_val_ < 0)//len tren
	{
		if (map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != DOT_TILE && map_data.tile[y1][x1] != HUNTER_MODE_TILE && map_data.tile[y1][x1] != GHOST_DOOR_TILE ||
			map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != DOT_TILE && map_data.tile[y1][x2] != HUNTER_MODE_TILE && map_data.tile[y1][x2] != GHOST_DOOR_TILE)
		{
			y_pos_ = (y1 + 1) * TILE_SIZE;
			y_val_ = 0;
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;
}

