#include "Blinky.h"
#include <SDL_timer.h>

Blinky::Blinky()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 430;//toa do
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

	blinky_status_ = 0;
	arrow_status_ = 0;

	on_wall = false;
}

Blinky::~Blinky()
{
	//todo
}

void Blinky::SetClip()
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

bool Blinky::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / NUM_OF_GHOST_FRAME;
		height_frame_ = rect_.h;
	}
	return false;
}

void Blinky::Show(SDL_Renderer* des)
{
	if (arrow_status_ == WALK_LEFT)
	{
		LoadImg("image//blinky_img//blinky_left.png", des);
	}
	else if (arrow_status_ == WALK_RIGHT)
	{
		LoadImg("image//blinky_img//blinky_right.png", des);
	}
	else if (arrow_status_ == WALK_UP)
	{
		LoadImg("image//blinky_img//blinky_up.png", des);
	}
	else if (arrow_status_ == WALK_DOWN)
	{
		LoadImg("image//blinky_img//blinky_down.png", des);
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

void Blinky::DoPlayer(Map& map_data)
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

struct Cell {
	int x, y;     // Coordinates of the cell
	int f, g, h;  // Values used in A* algorithm
	Cell* parent; // Pointer to the parent cell
};

struct CompareCell {
	bool operator()(const Cell* a, const Cell* b) const {
		return a->f > b->f;
	}
};

void Blinky::BlinkyMove1(Map& map_data, std::pair<int, int> pinky_coor, std::pair<int, int> pac_coor) {
	// Get the current time
	Uint32 currentTime = SDL_GetTicks();
	// If a second has passed since the last update
	if (currentTime - lastUpdateTime >= 2000) {
		lastUpdateTime = currentTime;

		// Create a priority queue to store open cells
		std::priority_queue<Cell*, std::vector<Cell*>, CompareCell> openCells;

		// Create a 2D array to keep track of visited cells
		std::vector<std::vector<bool>> visited(MAX_MAP_X, std::vector<bool>(MAX_MAP_Y, false));

		// Get the current position of Pinky
		int startX = pinky_coor.first;
		int startY = pinky_coor.second;

		// Get the position of Pacman
		int targetX = pac_coor.first;
		int targetY = pac_coor.second;

		// Create the start cell
		Cell* startCell = new Cell{ startX, startY, 0, 0, 0, nullptr };

		// Push the start cell into the priority queue
		openCells.push(startCell);

		// Loop until openCells is empty
		while (!openCells.empty()) {
			// Get the cell with the lowest f value
			Cell* currentCell = openCells.top();
			openCells.pop();

			// Mark the current cell as visited
			visited[currentCell->x][currentCell->y] = true;

			// Check if the current cell is the target cell
			if (currentCell->x == targetX && currentCell->y == targetY) {
				// Reconstruct the path
				std::vector<std::pair<int, int>> path;
				while (currentCell != nullptr) {
					path.push_back({ currentCell->x, currentCell->y });
					currentCell = currentCell->parent;
				}
				// Move Pinky along the path (path[1] is the next cell after the current position)
				if (path.size() > 1) {
					int nextX = path[1].first;
					int nextY = path[1].second;
					if (nextX < startX) {
						arrow_status_ = WALK_LEFT;
					}
					else if (nextX > startX) {
						arrow_status_ = WALK_RIGHT;
					}
					else if (nextY < startY) {
						arrow_status_ = WALK_UP;
					}
					else if (nextY > startY) {
						arrow_status_ = WALK_DOWN;
					}
				}

				// Cleanup memory
				for (auto& row : visited) {
					row.clear();
				}
				visited.clear();
				while (!openCells.empty()) {
					delete openCells.top();
					openCells.pop();
				}
				delete startCell;

				return;
			}

			// Generate neighboring cells
			for (int i = -1; i <= 1; ++i) {
				for (int j = -1; j <= 1; ++j) {
					if (i == 0 && j == 0) continue; // Skip the current cell
					int nextX = currentCell->x + i;
					int nextY = currentCell->y + j;

					// Check if the neighboring cell is valid
					if (nextX >= 0 && nextX < MAX_MAP_X && nextY >= 0 && nextY < MAX_MAP_Y &&
						map_data.tile[nextX][nextY] != 1 && !visited[nextX][nextY]) {
						// Calculate the g, h, and f values for the neighboring cell
						int g = currentCell->g + 1; // Assuming each step has a cost of 1
						int h = std::abs(nextX - targetX) + std::abs(nextY - targetY); // Manhattan distance heuristic
						int f = g + h;

						// Create the neighboring cell
						Cell* nextCell = new Cell{ nextX, nextY, f, g, h, currentCell };

						// Push the neighboring cell into the priority queue
						openCells.push(nextCell);
					}
				}
			}
		}
	}

	 //No path found, handle this case...
}



/*bool Blinky::isValid(int x, int y, Map& map_data) {
	return (x >= 0 && x < MAX_MAP_Y && y >= 0 && y < MAX_MAP_X && map_data.tile[x][y] != 1);
}

Node* Blinky::findShortestPath(Map& map_data, std::pair<int, int> start, std::pair<int, int> target) {
	// Khởi tạo hàng đợi cho BFS
	std::queue<Node*> q;
	q.push(new Node{ start.first, start.second, nullptr });
	// Tạo mảng visited để đánh dấu các ô đã được duyệt qua
	bool visited[MAX_MAP_Y][MAX_MAP_X] = { false };
	if (isValid(start.first, start.second, map_data)) {
		visited[start.first][start.second] = true;
	}
	else {
		// Xử lý trường hợp ngoại lệ khi start không nằm trong phạm vi của bản đồ
	}


	// Duyệt qua các ô lân cận của start để tìm đường đi ngắn nhất đến target
	while (!q.empty()) {
		Node* current = q.front();
		q.pop();

		// Nếu tìm thấy target, trả về nút cuối cùng trên đường đi
		if (current->x == target.first && current->y == target.second) {
			return current;
		}

		// Kiểm tra các ô lân cận của current
		int dx[] = { -1, 1, 0, 0 };
		int dy[] = { 0, 0, -1, 1 };
		for (int i = 0; i < 4; ++i) {
			int nextX = current->x + dx[i];
			int nextY = current->y + dy[i];

			// Nếu ô lân cận hợp lệ và chưa được duyệt, thêm vào hàng đợi
			if (isValid(nextX, nextY, map_data) && !visited[nextX][nextY]) {
				visited[nextX][nextY] = true;
				q.push(new Node{ nextX, nextY, current });
			}
		}
	}

	// Nếu không tìm thấy đường đi, trả về nullptr
	return nullptr;
}

void Blinky::BlinkyMove1(Map& map_data, std::pair<int, int> blinky_coor, std::pair<int, int> pac_coor) {
	// Tìm đường đi ngắn nhất từ vị trí hiện tại đến vị trí của Pac-Man
	Node* shortestPath = findShortestPath(map_data, blinky_coor, pac_coor);

	// Nếu có đường đi hợp lệ, di chuyển theo đường đi đó
	if (shortestPath != nullptr) {
		int nextX = shortestPath->x;
		int nextY = shortestPath->y;
		std::cout << " nextXzzzzz, nextY: " << nextX << " " << nextY << std::endl;
		std::cout << " y_pos_, y_pos_: " << x_pos_ << " " << y_pos_ << std::endl;
		// Xác định hướng di chuyển từ vị trí hiện tại đến ô tiếp theo
		if (nextX < blinky_coor.first) {
			arrow_status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		else if (nextX > blinky_coor.first) {
			arrow_status_ = WALK_RIGHT;
		}
		else if (nextY < blinky_coor.second) {
			arrow_status_ = WALK_UP;
		}
		else if (nextY > blinky_coor.second) {
			arrow_status_ = WALK_DOWN;
		}

		blinky_coor.first = nextX;
		blinky_coor.second = nextY;

		// Giải phóng bộ nhớ của đường đi
		while (shortestPath->parent != nullptr) {
			Node* temp = shortestPath;
			shortestPath = shortestPath->parent;
			delete temp;
		}
	}
}*/


void Blinky::CheckToMap(Map& map_data)
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
				on_wall = true;
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
				on_wall = true;
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
			on_wall = true;
		}
	}
	else if (y_val_ < 0)//len tren
	{
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

std::pair<int, int> Blinky::Get_current_coordinates_(Map& map_data)
{
	int x1 = (x_pos_ + x_val_ - SIDE_LEFT + 5) / TILE_SIZE;//o thu bao nhieu
	int y1 = (y_pos_ + y_val_ + 5) / TILE_SIZE;
	std::pair<int, int> coor = { x1,y1 };
	return coor;
}

//Lay o ma ghost dang dung

void Blinky::BlinkyMove(Map& map_data)
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
		if (arrow_status_ == WALK_RIGHT && blinky_status_ != WALK_RIGHT)
		{
			if (map_data.tile[y1][x2 + 1] == BLANK_TILE && map_data.tile[y2][x2 + 1] == BLANK_TILE ||
				map_data.tile[y1][x2 + 1] == DOT_TILE && map_data.tile[y2][x2 + 1] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
				if (blinky_status_ == WALK_UP) y_pos_ -= PACMAN_SPEED;
				else if (blinky_status_ == WALK_DOWN) y_pos_ += PACMAN_SPEED;
				blinky_status_ = WALK_RIGHT;

			}
		}
		else if (arrow_status_ == WALK_LEFT && blinky_status_ != WALK_LEFT)
		{
			if (map_data.tile[y1][x1 - 1] == BLANK_TILE && map_data.tile[y2][x1 - 1] == BLANK_TILE ||
				map_data.tile[y1][x1 - 1] == DOT_TILE && map_data.tile[y2][x1 - 1] == DOT_TILE)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
				if (blinky_status_ == WALK_UP) y_pos_ -= PACMAN_SPEED;
				else if (blinky_status_ == WALK_DOWN) y_pos_ += PACMAN_SPEED;
				blinky_status_ = WALK_LEFT;
			}

		}
		else if (arrow_status_ == WALK_UP && blinky_status_ != WALK_UP)
		{
			if (map_data.tile[y1 - 1][x1] == BLANK_TILE && map_data.tile[y1 - 1][x2] == BLANK_TILE ||
				map_data.tile[y1 - 1][x1] == DOT_TILE && map_data.tile[y1 - 1][x2] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 0;
				input_type_.up_ = 1;
				input_type_.down_ = 0;
				if (blinky_status_ == WALK_RIGHT) {
					x_pos_ = ((x_pos_ - 300) / TILE_SIZE + 1) * TILE_SIZE + 300 - (TILE_SIZE - PACMAN_SPEED);
				}
				else if (blinky_status_ == WALK_LEFT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE - 1) * TILE_SIZE + 300 + (TILE_SIZE - PACMAN_SPEED);
				blinky_status_ = WALK_UP;
			}

		}
		else if (arrow_status_ == WALK_DOWN && blinky_status_ != WALK_DOWN)
		{
			if (map_data.tile[y2 + 1][x1] == BLANK_TILE && map_data.tile[y2 + 1][x2] == BLANK_TILE ||
				map_data.tile[y2 + 1][x1] == DOT_TILE && map_data.tile[y2 + 1][x2] == DOT_TILE)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 1;
				if (blinky_status_ == WALK_RIGHT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE + 1) * TILE_SIZE + 300 - (TILE_SIZE - PACMAN_SPEED);
				else if (blinky_status_ == WALK_LEFT) x_pos_ = ((x_pos_ - 300) / TILE_SIZE - 1) * TILE_SIZE + 300 + (TILE_SIZE - PACMAN_SPEED);
				blinky_status_ = WALK_DOWN;
			}
		}
	}
	else {
		if (arrow_status_ == WALK_LEFT) {
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			blinky_status_ = WALK_LEFT;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_RIGHT) {
			input_type_.left_ = 0;
			input_type_.right_ = 1;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			blinky_status_ = WALK_RIGHT;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_UP) {
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 1;
			input_type_.down_ = 0;
			blinky_status_ = WALK_UP;
			on_wall = false;
		}
		else if (arrow_status_ == WALK_DOWN) {
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 1;
			blinky_status_ = WALK_DOWN;
			on_wall = false;
		}
	}

}