#pragma once
#ifndef GHOST_H_
#define GHOST_H_
#include "CommonFunc.h"
#include "BaseObject.h"

class Ghost : public BaseObject
{
private:
	int frame_;
	int width_frame_;
	int height_frame_;
	SDL_Rect frame_clip_[NUM_OF_FRAME_MOVE];
	SDL_Rect frame_clip_scary[NUM_OF_FRAME_MOVE];

	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	int map_x_;
	int map_y_;

	int arrow_status_;
	int alive_status_;

	Input input_type_;

	bool paused_;

	bool on_wall;
	bool eat_pacman;
	std::map<std::pair<int, int>, std::pair<int, int>>parent;
	std::map<std::pair<int, int>, bool> vet;
	int goal_x; int goal_y;
	int dx[4]; int dy[4];
	int random_direction;

public:
	Ghost();
	~Ghost();

	enum move_type
	{
		WALK_LEFT = 0,
		WALK_RIGHT = 1,
		WALK_UP = 2,
		WALK_DOWN = 3,
	};

	enum alive_type
	{
		EYES = 0,
		SCARY = 1,
		TIMES_UP = 2,
		ALIVE = 3,
	};

	enum type_of_ghost
	{
		BLINKY = 0,
		PINKY = 1,
		INKY = 2,
		CLYDE = 3,
		//FRIENDLY
	};

	void Set_where_start(int x, int y) { x_pos_ = rect_.x = x; y_pos_ = rect_.y = y; };
	void SetClip();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des, int type_of_ghost);

	int Get_width_frame() const { return width_frame_; };
	int Get_height_frame() const { return height_frame_; };

	void DoPlayer(Map map_data);
	void CheckToMap(Map map_data);

	void Set_paused(bool pau) { paused_ = pau; };

	std::pair <int, int> Get_current_coordinates_(Map map_data);

	void BFS(int x, int y, Map map);
	bool CheckEndCell(int i, int j);
	void Move(Map map_data);
	void set_goal_x(int x) { goal_x = x; };
	void set_goal_y(int y) { goal_y = y; };

	std::pair<int, int> Get_rect() { std::pair<int, int> c; c.first = rect_.x; c.second = rect_.y; return c; };

	int Get_alive_status() { return alive_status_; };
	void Set_alive_status(int x) {
		alive_status_ = x;
		if (x == 1 || x == 0) eat_pacman = false;
		else if (x == 3) eat_pacman = true;

	};

	void set_eat_pacman(bool u) {
		eat_pacman = u;
	}
	bool get_eat_pacman() {
		return eat_pacman;
	}
	int get_goal_x() { return goal_x; }
	int get_goal_y() { return goal_y; }
};

#endif