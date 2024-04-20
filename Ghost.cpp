#include "Ghost.h"

Ghost::Ghost()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 370;
	y_pos_ = 240;
	frame_ = 0;
	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	paused_ = false;
}

Ghost::~Ghost()
{
	//todo
}

void Ghost::SetClip()
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

bool Ghost::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / NUM_OF_GHOST_FRAME;
		height_frame_ = rect_.h;
	}
	return false;
}

void Ghost::Show(SDL_Renderer* des)
{
	if (input_type_.left_ == 1)
	{
		LoadImg("image//pinky//pinky_left.png", des);
	}
	else if (input_type_.right_ == 1)
	{
		LoadImg("image//pinky//pinky_right.png", des);
	}
	else if (input_type_.up_ == 1)
	{
		LoadImg("image//pinky//pinky_up.png", des);
	}
	else if (input_type_.down_ == 1)
	{
		LoadImg("image//pinky//pinky_down.png", des);
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

void Ghost::DoPlayer(Map& map_data)
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

void Ghost::CheckToMap(Map& map_data)
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

/*void Ghost::BFS(Map& map_data, std::pair<int, int> ghost_coordinates, std::pair<int, int> pac_coordinates)
{
	bool visited[MAX_MAP_Y][MAX_MAP_X];
	int dx[4] = { -1, 0, 0, 1 };
	int dy[4] = { 0, -1, 1, 0 };
	std::map<std::pair<int, int>, std::pair<int, int>> vet;

	memset(visited, false, sizeof(visited));

	//bat dau thuat toan bfs
	std::queue<std::pair<int, int>> qe;
	qe.push({ ghost_coordinates.first,ghost_coordinates.second });

	std::pair<int, int> v;
	int check_to_destination = false;
	while (!qe.empty())
	{
		v = qe.front();
		qe.pop();
		for (int k = 0; k < 4; k++)
		{
			int i1 = v.first + dx[k];
			int j1 = v.second + dy[k];
			if (visited[i1][j1] == false && map_data.tile[i1][j1] != 1)
			{
				qe.push({ i1, j1 });
				vet[{i1, j1}] = { v.first, v.second };
				if (i1 == pac_coordinates.first && j1 == pac_coordinates.second)
				{
					//ok =1
					check_to_destination = true;
					break;
				}
				visited[i1][j1] = true;
			}
		}
		if (check_to_destination == true) break;
	}

	//thuc hien duong di theo thuat toan bfs

	std::pair<int, int> path;
	path = { pac_coordinates.first, pac_coordinates.second };
	while (path.first != ghost_coordinates.first || path.second != ghost_coordinates.second)
	{
		way.push_back(path);
		path = vet[{path.first, path.second}];
	}
	reverse(way.begin(), way.end());

	std::cout << "di tu " << ghost_coordinates.first << " " << ghost_coordinates.second;
	std::cout << " den " << pac_coordinates.first << " " << pac_coordinates.second << std::endl;
	for (auto it : way)
	{
		std::cout << it.first << " " << it.second << std::endl;

	}
	std::cout << "Het roi nha" << std::endl;
}*/
//BFS khong tra ve gia tri gi

/*std::vector<std::pair<int, int>> Ghost::BFS(Map& map_data, std::pair<int, int> ghost_coordinates, std::pair<int, int> pac_coordinates)
{
	bool visited[MAX_MAP_Y][MAX_MAP_X];
	memset(visited, false, sizeof(visited));
	std::queue<std::pair<int, int>> q;
	std::vector<std::vector<std::pair<int, int>>> prev(MAX_MAP_Y, std::vector<std::pair<int, int>>(MAX_MAP_X, { -1, -1 }));
	visited[ghost_coordinates.first][ghost_coordinates.second] = true;
	q.push({ ghost_coordinates.first, ghost_coordinates.second });

	while (!q.empty()) {
		std::pair<int, int> p = q.front();
		q.pop();

		if (p.first == pac_coordinates.first && p.second == pac_coordinates.second) {
			break;
		}

		for (int i = 0; i < 4; i++) {
			int newX = p.first + dx[i];
			int newY = p.second + dy[i];

			// Kiem tra xem co the di chuyen đen o (newX, newY) hay khong
			if (newX >= 0 && newX < MAX_MAP_Y && newY >= 0 && newY < MAX_MAP_X && map_data.tile[newX][newY] != 1 && !visited[newX][newY]) {
				visited[newX][newY] = true;
				prev[newX][newY] = p;
				q.push({ newX, newY });
			}
		}
	}

	std::vector<std::pair<int, int>> path;
	for (std::pair<int, int> at = { pac_coordinates.first, pac_coordinates.second }; at != std::make_pair(-1, -1); at = prev[at.first][at.second]) {
		path.push_back(at);
	}
	reverse(path.begin(), path.end());
	std::cout << "di tu " << ghost_coordinates.first << " " << ghost_coordinates.second;
	std::cout << " den " << pac_coordinates.first << " " << pac_coordinates.second << std::endl;
	for (auto it : path)
	{
		std::cout << it.first << " " << it.second << std::endl;
		
	}
    std::cout << "Het roi nha" << std::endl;
	return path;
}*/
//BFS tra ve vector luu duong di

/*std::pair<int, int> Ghost::Get_current_coordinates_(Map& map_data)
{
	CheckToMap(map_data);
	int x1 = (x_pos_ + x_val_ - SIDE_LEFT + 5) / TILE_SIZE;//o thu bao nhieu
	int x2 = (x_pos_ - SIDE_LEFT + width_frame_) / TILE_SIZE;

	int y1 = (y_pos_ + y_val_ + 5) / TILE_SIZE;
	int y2 = (y_pos_ + height_frame_) / TILE_SIZE;
	std::pair<int, int> coor = { x1,y1 };
	return coor;
}*/
//Lay o ma ghost dang dung

/*void Ghost::DoThePath(Map& map_data, Pacman pac)
{
	std::pair<int, int> pac_coor = pac.Get_current_coordinates_(map_data);
	std::pair<int, int> ghost_coor = Get_current_coordinates_(map_data);
	std::vector<std::pair<int, int>> way = BFS(map_data, ghost_coor, pac_coor);
	//BFS(map_data, ghost_coor, pac_coor);
	std::pair<int, int> check;
	//for (auto it : way)
	//{
		//std::cout << it.first << " " << it.second << std::endl;
	if (way.size() > 1)
	{
		if (ghost_coor.first < way[1].first)
		{
			input_type_.left_ = 0;
			input_type_.right_ = 1;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			DoPlayer(map_data);
		}
		else if (ghost_coor.first > way[1].first)
		{
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			DoPlayer(map_data);
		}
		else if (ghost_coor.second > way[1].second)
		{
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 1;
			input_type_.down_ = 0;
			DoPlayer(map_data);
		}
		else if (ghost_coor.second < way[1].second)
		{
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 1;
			DoPlayer(map_data);
		}
	}

	//}
	way.clear();
}*/
//di chuyen tung o
