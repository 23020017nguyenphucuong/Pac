#pragma once
#ifndef INKY_H_
#define INKY_H_
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Timer.h"
class Inky : public BaseObject
{
private:
	int frame_;
	int width_frame_;
	int height_frame_;
	SDL_Rect frame_clip_[NUM_OF_FRAME_MOVE];

	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	int map_x_;
	int map_y_;

	int type_move_;

	int animation_a_;
	int animation_b_;

	int arrow_status_;
	int scary_status_;

	Uint32 lastUpdateTime;

	Input input_type_;

	//std::vector<std::pair<int, int>> way;

	bool paused_;

	bool initialDirectionSet;
	bool on_wall;

	std::map<std::pair<int, int>, std::pair<int, int>>parent;
	std::map<std::pair<int, int>, bool> vet;
	int goal_x; int goal_y;
	int dx[4]; int dy[4];
	int random_huong;
	Timer Time_Delay;
	
public:
	Inky();
	~Inky();

	enum move_type
	{
		WALK_LEFT = 0,
		WALK_RIGHT = 1,
		WALK_UP = 2,
		WALK_DOWN = 3,
	};

	enum allive_type
	{
		EYES = 0,
		SCARY = 1,
		TIMES_UP = 2,
		ALLIVE = 3,
	};

	//void InkyMove1(Map& map_data);

	void Set_x_val(const float& xv) { x_val_ = xv; };
	void Set_y_val(const float& yv) { y_val_ = yv; };

	void Set_x_pos(const float& xp) { x_pos_ = xp; };
	void Set_y_pos(const float& yp) { y_pos_ = y_val_; };
	float Get_x_pos() const { return x_pos_; };
	float Get_y_pos() const { return y_pos_; };

	void SetMapXY(const int mp_x, const int mp_y) { map_x_ = mp_x; map_y_ = mp_y; };
	void SetClip();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);

	int Get_width_frame() const { return width_frame_; };
	int Get_height_frame() const { return height_frame_; };

	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);

	void Set_type_move(const int& typemove) { type_move_ = typemove; };
	void Set_animation_pos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a; animation_b_ = pos_b; };

	void Set_input_left(const int& ip_left) { input_type_.left_ = ip_left; };
	void Set_input_right(const int& ip_right) { input_type_.right_ = ip_right; };
	void Set_input_up(const int& ip_up) { input_type_.up_ = ip_up; };
	void Set_input_down(const int& ip_down) { input_type_.down_ = ip_down; };

	void Set_paused(bool pau) { paused_ = pau; };

	std::pair <int, int> Get_current_coordinates_();
	void Find(int x, int y,  Map& map);
	bool kt(int i, int j);
	void Move();
	void set_goal_x(int x) {
		goal_x = x;
	}
	void set_goal_y(int y) {
		goal_y = y;
	}

};

#endif