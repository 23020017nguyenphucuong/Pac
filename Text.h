#pragma once
#ifndef TEXT_H_
#define TEXT_H_
#include "CommonFunc.h"

class Text
{
public:
	Text();
	~Text();

	enum TextColor
	{
		YELLOW_TEXT = 0,
		WHITE_TEXT = 1,
	};

	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
	void Free();

	void SetColor(Uint8 red, Uint8 green, Uint8 blue);
	void SetColor(int type);

	void RenderText(SDL_Renderer* screen, 
		            int xp, int yp,
		            SDL_Rect* clip = NULL,
		            double angle = 0.0, 
		            SDL_Point* center = NULL, 
		            SDL_RendererFlip flip = SDL_FLIP_NONE);

	int GetWidth() { return width_; };
	int GetHeight() { return height_; };

	void SetText(std::string text) { str_val_ = text; };
private:
	std::string str_val_;
	SDL_Color text_color_;
	SDL_Texture* texture_;
	int width_;
	int height_;
};

#endif 
