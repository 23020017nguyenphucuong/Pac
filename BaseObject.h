#pragma once
#pragma once
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunc.h"

class BaseObject
{
protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;
public:
	//constructor and destructor
	BaseObject();
	~BaseObject();
	//set dia chi
	void setRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }
	//lay dia chi
	SDL_Rect getRect() const { return rect_; }
	//tra ve doi tuong
	SDL_Texture* getObject() const { return p_object_; }

	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void ApplyRender(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void Free();

};

#endif