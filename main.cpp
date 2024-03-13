#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"

static BaseObject g_background;

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("PacMan Demo by NPC",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (g_window == NULL) success = false;
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
		}
	}
	return success;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("image//level.png", g_screen);
	if (ret == false) return false;
	return true;
}

void Close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (InitData() == false) return -1;
	if (LoadBackground() == false) return -1;

	//tai map
	GameMap game_map;

	game_map.LoadMap();
	game_map.LoadTiles(g_screen);

	bool is_quit = false;

	g_background.setRect(300, 0);

	while (!is_quit)
	{
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT) is_quit = true;
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		//g_background.ApplyRender(g_screen, NULL);

		game_map.DrawMap(g_screen);

		SDL_RenderPresent(g_screen);
	}

	Close();

	return 0;
}