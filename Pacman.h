#pragma once
#ifndef PACMAN_OBJECT_H_
#define PACMAN_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class Pacman : public BaseObject
{
private:
	float x_val_;//vi tri
	float y_val_;

	float x_pos_;//vi tri hien tai
	float y_pos_;

	int width_frame_;//kich thuoc cua frame
	int height_frame_;

	SDL_Rect frame_clip_[NUM_OF_FRAME_MOVE];//luu cac frame
	SDL_Rect frame_clip_die_[NUM_OF_FRAME_DIE];
	Input input_type_;//luu trang thai
	int frame_;//luu dia chi cua frame, dang o frame nao
	int frame_die_;

	int pacman_status_;//biet duoc pacman dang di chuyen sang ben nao
	int arrow_status_;//biet duoc mui ten dang chi ve phia nao
	int alive_status_;

	BaseObject arrow_direct[4];

	bool on_wall;

	int score;
	int number_of_dot;

	int hunter_mode;

	bool paused_;

	int dx[4] = { -1, 0, 0, 1 };
	int dy[4] = { 0, -1, 1, 0 };

	int eat_dot_sound;

	bool eat_boss1;
	bool eat_boss2;
	bool eat_boss3;
	bool eat_boss4;
	bool eat_boss5;

	int start_time_strong;
	int end_time_strong;
	int mang;
public:
	Pacman();
	~Pacman();

	enum move_type
	{
		WALK_LEFT = 2,
		WALK_RIGHT = 3,
		WALK_UP = 4,
		WALK_DOWN = 5,
	};
	enum alive_type
	{
		DIED = 0,
		ALIVE = 1,
	};

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);//show tung frame
	void ShowDie(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);//xu li di chuyen
	void SetClips();//tao hieu ung animation


	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);

	void ShowArrow(SDL_Renderer* des);
	void ArrowImgInit(SDL_Renderer* des);

	void PacmanMove(Map map_data);

	Uint32 Get_score() { return score; };
	void Set_score(Uint32 x) { score = x; }
	int Get_num_of_dot() { return number_of_dot; };

	std::pair <int, int> Get_current_coordinates_(Map map_data);

	int Get_move_status() const { return arrow_status_; };
	void Set_paused(bool pau) { paused_ = pau; };

	int Get_pacman_status() { return pacman_status_; };
	void Set_pacman_status(int n) { pacman_status_ = n; };
	void PacDie() { pacman_status_ = DIED; };

	std::pair<int, int> Get_rect() { std::pair<int, int> c; c.first = rect_.x; c.second = rect_.y; return c; };

	void Set_where_start(int x, int y) { x_pos_ = rect_.x = x; y_pos_ = rect_.y = y; };
	void Set_input_type(int l, int r, int u, int d) { input_type_.left_ = l; input_type_.right_ = r; input_type_.up_ = u; input_type_.down_ = d; };
	void Set_original_state();

	void Set_hunter_mode(int x) { hunter_mode = x; };
	bool Is_hunter_mode() { return hunter_mode; };

	void set_eat_boss(int x, bool u) {
		if (x == 1) eat_boss1 = u;
		else if (x == 2) eat_boss2 = u;
		else if (x == 3) eat_boss3 = u;
		else if (x == 4) eat_boss4 = u;
		else if (x == 5) eat_boss5 = u;
	}
	bool get_eat_boss(int x) {
		if (x == 1) return eat_boss1;
		else if (x == 2) return eat_boss2;
		else if (x == 3) return eat_boss3;
		else if (x == 4) return eat_boss4;
		else if (x == 5) return eat_boss5;
	}
	void reduce_mang() { mang--; }
	int get_mang() { return mang; }
	void set_mang(int x) { mang = x; }
};

#endif