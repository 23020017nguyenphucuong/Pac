#include "Pinky.h"
#include <SDL_timer.h>

Pinky::Pinky()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 340;//toa do
	y_pos_ = 240;
	frame_ = 0;
	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	paused_ = false;
	lastUpdateTime = 0;

	on_wall = false;
	initialDirectionSet = false;
}

Pinky::~Pinky()
{
	//todo
}

void Pinky::SetClip()
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

bool Pinky::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / NUM_OF_GHOST_FRAME;
		height_frame_ = rect_.h;
	}
	return false;
}

void Pinky::Show(SDL_Renderer* des)
{
	if (input_type_.left_ == 1)
	{
		LoadImg("image//pinky_img//pinky_left.png", des);
	}
	else if (input_type_.right_ == 1)
	{
		LoadImg("image//pinky_img//pinky_right.png", des);
	}
	else if (input_type_.up_ == 1)
	{
		LoadImg("image//pinky_img//pinky_up.png", des);
	}
	else if (input_type_.down_ == 1)
	{
		LoadImg("image//pinky_img//pinky_down.png", des);
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

void Pinky::DoPlayer(Map& map_data)
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

void Pinky::PinkyMove1(Map& map_data)
{
	std::pair<int, int> coor = Get_current_coordinates_();
	int x = coor.first;
	int y = coor.second;

	bool upIsWall = (y > 0 && map_data.tile[x][y - 1] != 1);
	bool downIsWall = (y < MAX_MAP_Y - 1 && map_data.tile[x][y + 1] != 1);
	bool leftIsWall = (x > 0 && map_data.tile[x - 1][y] != 1);
	bool rightIsWall = (x < MAX_MAP_X - 1 && map_data.tile[x + 1][y] != 1);
	CheckToMap(map_data);

	// Nếu chưa có hướng được chọn ban đầu, chọn một hướng ngẫu nhiên có thể di chuyển được
	if (!initialDirectionSet)
	{
		std::vector<int> available_directions;
		if (!upIsWall) available_directions.push_back(WALK_UP);
		if (!downIsWall) available_directions.push_back(WALK_DOWN);
		if (!leftIsWall) available_directions.push_back(WALK_LEFT);
		if (!rightIsWall) available_directions.push_back(WALK_RIGHT);

		if (!available_directions.empty())
		{
			int randomIndex = rand() % available_directions.size();
			arrow_status_ = available_directions[randomIndex];
			input_type_.left_ = (arrow_status_ == WALK_LEFT);
			input_type_.right_ = (arrow_status_ == WALK_RIGHT);
			input_type_.up_ = (arrow_status_ == WALK_UP);
			input_type_.down_ = (arrow_status_ == WALK_DOWN);
		}

		initialDirectionSet = true;
	}
	// Nếu đang ở trạng thái trên tường, chọn một hướng di chuyển ngẫu nhiên khác hướng hiện tại
	else if (on_wall)
	{
		std::vector<int> available_directions;
		if (!upIsWall && arrow_status_ != WALK_DOWN) available_directions.push_back(WALK_UP);
		if (!downIsWall && arrow_status_ != WALK_UP) available_directions.push_back(WALK_DOWN);
		if (!leftIsWall && arrow_status_ != WALK_RIGHT) available_directions.push_back(WALK_LEFT);
		if (!rightIsWall && arrow_status_ != WALK_LEFT) available_directions.push_back(WALK_RIGHT);

		if (!available_directions.empty())
		{
			int randomIndex = rand() % available_directions.size();
			arrow_status_ = available_directions[randomIndex];
			input_type_.left_ = (arrow_status_ == WALK_LEFT);
			input_type_.right_ = (arrow_status_ == WALK_RIGHT);
			input_type_.up_ = (arrow_status_ == WALK_UP);
			input_type_.down_ = (arrow_status_ == WALK_DOWN);
		}
	}
}





void Pinky::CheckToMap(Map& map_data)
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
		on_wall = false;
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
				on_wall = true;
			}
		}

	}
	else if (x_val_ < 0)//sang trai
	{
		on_wall = false;
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
				on_wall = true;
			}
		}
	}
	else if (y_val_ > 0)//xuong duoi
	{
		on_wall = false;
		if (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != DOT_TILE && map_data.tile[y2][x1] != HUNTER_MODE_TILE && map_data.tile[y2][x1] != GHOST_DOOR_TILE ||
			map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != DOT_TILE && map_data.tile[y2][x2] != HUNTER_MODE_TILE && map_data.tile[y2][x2] != GHOST_DOOR_TILE)
		{
			y_pos_ = y2 * TILE_SIZE - height_frame_;
			y_val_ = 0;
			on_wall = true;
		}
	}
	else if (y_val_ < 0)//len tren
	{
		on_wall = false;
		if (map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != DOT_TILE && map_data.tile[y1][x1] != HUNTER_MODE_TILE && map_data.tile[y1][x1] != GHOST_DOOR_TILE ||
			map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != DOT_TILE && map_data.tile[y1][x2] != HUNTER_MODE_TILE && map_data.tile[y1][x2] != GHOST_DOOR_TILE)
		{
			y_pos_ = (y1 + 1) * TILE_SIZE;
			y_val_ = 0;
			on_wall = true;
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;
}

/*void Pinky::PinkyMove(Map& map_data)
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
		if (arrow_status_ == WALK_RIGHT && pinky_status_ != WALK_RIGHT)
		{
			if (map_data.tile[y1][x2 + 1] == BLANK_TILE && map_data.tile[y2][x2 + 1] == BLANK_TILE ||
				map_data.tile[y1][x2 + 1] == DOT_TILE && map_data.tile[y2][x2 + 1] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
				if (pinky_status_ == WALK_UP) y_pos_ -= PACMAN_SPEED;
				else if (pinky_status_ == WALK_DOWN) y_pos_ += PACMAN_SPEED;
				pinky_status_ = WALK_RIGHT;

			}
		}
		else if (arrow_status_ == WALK_LEFT && pinky_status_ != WALK_LEFT)
		{
			if (map_data.tile[y1][x1 - 1] == BLANK_TILE && map_data.tile[y2][x1 - 1] == BLANK_TILE ||
				map_data.tile[y1][x1 - 1] == DOT_TILE && map_data.tile[y2][x1 - 1] == DOT_TILE)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
				if (pinky_status_ == WALK_UP) y_pos_ -= PACMAN_SPEED;
				else if (pinky_status_ == WALK_DOWN) y_pos_ += PACMAN_SPEED;
				pinky_status_ = WALK_LEFT;
			}

		}
		else if (arrow_status_ == WALK_UP && pinky_status_ != WALK_UP)
		{
			if (map_data.tile[y1 - 1][x1] == BLANK_TILE && map_data.tile[y1 - 1][x2] == BLANK_TILE ||
				map_data.tile[y1 - 1][x1] == DOT_TILE && map_data.tile[y1 - 1][x2] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 0;
				input_type_.up_ = 1;
				input_type_.down_ = 0;
				if (pinky_status_ == WALK_RIGHT) {
					x_pos_ = ((x_pos_ - 300) / TILE_SIZE + 1) * TILE_SIZE + 300 - (TILE_SIZE - PACMAN_SPEED);
				}
				else if (pinky_status_ == WALK_LEFT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE - 1) * TILE_SIZE + 300 + (TILE_SIZE - PACMAN_SPEED);
				pinky_status_ = WALK_UP;
			}

		}
		else if (arrow_status_ == WALK_DOWN && pinky_status_ != WALK_DOWN)
		{
			if (map_data.tile[y2 + 1][x1] == BLANK_TILE && map_data.tile[y2 + 1][x2] == BLANK_TILE ||
				map_data.tile[y2 + 1][x1] == DOT_TILE && map_data.tile[y2 + 1][x2] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 1;
				if (pinky_status_ == WALK_RIGHT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE + 1) * TILE_SIZE + 300 - (TILE_SIZE - PACMAN_SPEED);
				else if (pinky_status_ == WALK_LEFT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE - 1) * TILE_SIZE + 300 + (TILE_SIZE - PACMAN_SPEED);
				pinky_status_ = WALK_DOWN;
			}
		}
	}
	else {
		if (arrow_status_ == WALK_LEFT) {
			input_type_.left_ = 1;
			pinky_status_ = WALK_LEFT;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_RIGHT) {
			input_type_.right_ = 1;
			pinky_status_ = WALK_RIGHT;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_UP) {
			input_type_.up_ = 1;
			pinky_status_ = WALK_UP;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_DOWN) {
			input_type_.down_ = 1;
			pinky_status_ = WALK_DOWN;
			on_wall = false;
		}
	}

}*/

std::pair<int, int> Pinky::Get_current_coordinates_()
{
	int x1 = (x_pos_ + x_val_ - SIDE_LEFT + 5) / TILE_SIZE;//o thu bao nhieu
	int y1 = (y_pos_ + y_val_ + 5) / TILE_SIZE;
	std::pair<int, int> coor = { x1,y1 };
	return coor;
}
