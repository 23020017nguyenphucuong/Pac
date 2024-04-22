#pragma once
#ifndef Clyde_H_
#define Clyde_H_
#include "CommonFunc.h"
#include "BaseObject.h"

class Clyde : public BaseObject
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

	Uint32 lastUpdateTime;

	Input input_type_;

	//std::vector<std::pair<int, int>> way;

	bool paused_;
	int dx[4] = { -1, 0, 0, 1 };
	int dy[4] = { 0, -1, 1, 0 };

public:
	Clyde();
	~Clyde();

	enum move_type
	{
		DIED = -1,
		ALIVE = 0,
		SCARRY = 1,
		WALK_LEFT = 2,
		WALK_RIGHT = 3,
		WALK_UP = 4,
		WALK_DOWN = 5,
	};

	void ClydeMove1();

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

};

#endif