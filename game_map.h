#pragma once
#pragma once
#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 5
class TileMat : public BaseObject
{

public:
	TileMat() { ; }
	~TileMat() { ; }
};

class GameMap
{
private:
	Map game_map_;
	TileMat tile_mat[MAX_TILES + 1];
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap();
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
};

#endif