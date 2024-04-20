#include "game_map.h"
#include<iostream>
void GameMap::LoadMap()
{
	int k = 0;
	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for (int j = 0; j < MAX_MAP_X; j++)
		{
			game_map_.tile[i][j] = map01[k]; k++;
		}
	}
}

void GameMap::LoadTiles(SDL_Renderer* screen)
{
	char file_img[20];
	FILE* fp = NULL;

	for (int i = 0; i < MAX_TILES; i++)
	{
		sprintf_s(file_img, "map01/%d.png", i);

		fopen_s(&fp, file_img, "rb");
		if (fp == NULL)
		{
			continue;
		}


		tile_mat[i].LoadImg(file_img, screen);

	}
	fclose(fp);
}

void GameMap::DrawMap(SDL_Renderer* screen,Map &map_data)
{
	int x1 = MAP_CORNER_X1;
	int x2 = MAP_CORNER_X2;

	int y1 = MAP_CORNER_Y1;
	int y2 = MAP_CORNER_Y2;

	int map_x = 0;
	int map_y = 0;

	for (int i = y1; i < y2; i += TILE_SIZE)
	{
		map_x = 0;
		for (int j = x1; j < x2; j += TILE_SIZE)
		{
			int val = map_data.tile[map_y][map_x];
			if (val >= 0)
			{
				tile_mat[val].setRect(j, i);
				tile_mat[val].ApplyRender(screen, NULL);
			}
			map_x++;
		}
		map_y++;
	}
}