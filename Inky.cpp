#include "Inky.h"
#include <SDL_timer.h>

Inky::Inky()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 460;//toa do
	y_pos_ = 300;
	rect_.x = 460;
	rect_.y = 300;
	frame_ = 0;
	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	paused_ = false;
	lastUpdateTime = 0;

	initialDirectionSet = false;
	on_wall = false;
	scary_status_ = ALLIVE;
	
		random_huong = rand() % 4;
		if (random_huong == 0) {
			dx[0] = 1; dx[1] = -1; dx[2] = 0; dx[3] = 0;
			dy[0] = 0; dy[1] = 0; dy[2] = -1; dy[3] = 1;
		}
		else if (random_huong == 1) {
			dx[0] = -1; dx[1] = 1; dx[2] = 0; dx[3] = 0;
			dy[0] = 0; dy[1] = 0; dy[2] = 1; dy[3] = -1;
		}
		else if (random_huong == 2) {
			dx[3] = -1; dx[2] = 1; dx[1] = 0; dx[0] = 0;
			dy[3] = 0; dy[2] = 0; dy[1] = 1; dy[0] = -1;
		}
		else {
			dx[3] = 1; dx[2] = -1; dx[1] = 0; dx[0] = 0;
			dy[3] = 0; dy[2] = 0; dy[1] = -1; dy[0] = 1;
		}
		Time_Delay.start();
		
}

Inky::~Inky()
{
	//todo
}

void Inky::SetClip()
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

bool Inky::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / NUM_OF_GHOST_FRAME;
		height_frame_ = rect_.h;
	}
	return false;
}

void Inky::Show(SDL_Renderer* des)
{
	if (scary_status_ == EYES)
	{
		if (input_type_.left_ == 1)
		{
			LoadImg("image//ghost_scary_img//eyes_left.png", des);
		}
		else if (input_type_.right_ == 1)
		{
			LoadImg("image//ghost_scary_img//eyes_right.png", des);
		}
		else if (input_type_.up_ == 1)
		{
			LoadImg("image//ghost_scary_img//eyes_up.png", des);
		}
		else if (input_type_.down_ == 1)
		{
			LoadImg("image//ghost_scary_img//eyes_down.png", des);
		}
	}
	else if (scary_status_ == SCARY)
	{
		LoadImg("image//ghost_scary_img//ghost_scary.png", des);
	}
	else if (scary_status_ == TIMES_UP)
	{
		LoadImg("image//ghost_scary_img//ghost_times_up.png", des);
	}
	else if (scary_status_ == ALLIVE)
	{
		if (input_type_.left_ == 1)
		{
			LoadImg("image//inky_img//inky_left.png", des);
		}
		else if (input_type_.right_ == 1)
		{
			LoadImg("image//inky_img//inky_right.png", des);
		}
		else if (input_type_.up_ == 1)
		{
			LoadImg("image//inky_img//inky_up.png", des);
		}
		else if (input_type_.down_ == 1)
		{
			LoadImg("image//inky_img//inky_down.png", des);
		}
	}

	if (paused_ == false)
	{
		rect_.x = x_pos_;
		rect_.y = y_pos_;

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

void Inky::DoPlayer(Map& map_data)
{
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

/*void Inky::InkyMove1(Map& map_data)
{
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime - lastUpdateTime >= 2000)
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
*/
void Inky::CheckToMap(Map& map_data)
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

bool Inky::kt(int i, int j) {
	std::pair<int, int>my_pair = std::make_pair(i, j);
	std::pair<int, int>path = std::make_pair(goal_x, goal_y);
	if (my_pair == path)
		return false;
	return true;
}

void Inky::Find(int i, int j,  Map& map) {
		if (kt(i, j) == false) return;
		std::queue<std::pair<int, int>>qe;
		std::pair<int, int>my_pair = std::make_pair(i, j);
		vet[my_pair] = true; std::pair<int, int>v;
		qe.push(my_pair);
		while (!qe.empty()) {
			std::cout << "+1 lan chui vao vong while cua hang doi" << std::endl;
			v = qe.front(); qe.pop();
			for (int k = 0; k < 4; k++) {
				int i1 = v.first + dx[k]; int j1 = v.second + dy[k];
				std::pair<int, int>my_pair_1 = std::make_pair(i1, j1);
				std::pair<int, int>my_pair_2 = std::make_pair(v.first, v.second);
				if (i1 >= 0 && j1 >= 0 && i1 < MAX_MAP_X && j1 < MAX_MAP_Y && vet[my_pair_1] == false
					&& map.tile[j1][i1] != WALL_TILE) {
					std::cout << parent.size() << std::endl;
					qe.push(my_pair_1); parent[my_pair_1] = my_pair_2;
					std::pair<int, int>my_pair_3 = std::make_pair(goal_x, goal_y);
					if (vet[my_pair_3] == true) {
						Move(); 
						return;
					}
					vet[my_pair_1] = true;
				}
			}
		}
		for (auto& pair : vet) {
			pair.second = false;
		}
		std::cout << "Find done" << std::endl;
}
void Inky::Move() {

	std::pair<int, int>my_pair = Get_current_coordinates_();
	std::pair<int, int>path = std::make_pair(goal_x, goal_y);

	while (1) {
		if (parent[path] == my_pair) {
			//if (Time_Delay.get_sticks() > 500) {
				if (path.first < parent[path].first && path.second == parent[path].second) {
					input_type_.left_ = 1; arrow_status_ = WALK_LEFT;
				}
				else input_type_.left_ = 0;
				if (path.first > parent[path].first && path.second == parent[path].second) {
					input_type_.right_ = 1; arrow_status_ = WALK_RIGHT;
				}
				else input_type_.right_ = 0;
				if (path.first == parent[path].first && path.second > parent[path].second) {
					input_type_.down_ = 1; arrow_status_ = WALK_DOWN;
				}
				else input_type_.down_ = 0;
				if (path.first == parent[path].first && path.second < parent[path].second) {
					input_type_.up_ = 1; arrow_status_ = WALK_UP;
				}
				else input_type_.up_ = 0;
				//Time_Delay.start();
			//}
			break; 
		}
		path = parent[path];
		std::cout << "move done" << std::endl;
	}
	for (auto& pair : vet) {
		pair.second = false;
	}
	
}

std::pair<int, int> Inky::Get_current_coordinates_()
{
	int x1 = (rect_.x + x_val_ - SIDE_LEFT + 5) / TILE_SIZE;//o thu bao nhieu
	int y1 = (rect_.y + y_val_ + 5) / TILE_SIZE;
	std::pair<int, int> coor = { x1,y1 };
	return coor;
}
