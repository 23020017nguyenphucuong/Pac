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

	SDL_Rect frame_clip_[NUM_OF_FRAME];//luu cac frame
	Input input_type_;//luu trang thai
	int frame_;//luu dia chi cua frame, dang o frame nao
	int pacman_status_;//biet duoc pacman dang di chuyen sang ben nao
	int arrow_status_;//biet duoc mui ten dang chi ve phia nao

	BaseObject arrow_direct[4];

	bool on_wall;
public:
	Pacman();
	~Pacman();

	enum move_type
	{
		ALIVE = 0,
		DIED = 1,
		WALK_LEFT = 2,
		WALK_RIGHT = 3,
		WALK_UP = 4,
		WALK_DOWN = 5,
	};

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);//show tung frame
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);//xu li di chuyen
	void SetClips();//tao hieu ung animation

	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);

	void ShowArrow(SDL_Renderer* des);
	void ArrowImgInit(SDL_Renderer* des);

	void PacmanMove(Map& map_data);
};

#endif