#pragma once
#pragma once
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_
#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <vector>
#include <utility>
#include <map>
#include <queue>
#include <iostream>
#include <SDL_timer.h>
#include <random>
#include <cmath>
#include <chrono>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Chunk* g_sound_game[6];
static Mix_Chunk* g_sound_pac[6];
static Mix_Music* g_sound_music;

//screen
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 660;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 0;
const int COLOR_KEY_G = 0;
const int COLOR_KEY_B = 0;

const int CONTROL_COLOR_TRANSPARENT = 0;

//frame
#define NUM_OF_FRAME_MOVE 9
#define NUM_OF_FRAME_DIE 33
#define NUM_OF_GHOST_FRAME 6

//game map
#define MAX_TILES 11
#define TILE_SIZE 30
#define MAX_MAP_X 19
#define MAX_MAP_Y 22
#define MAP_CORNER_X1 100
#define MAP_CORNER_X2 670
#define MAP_CORNER_Y1 0
#define MAP_CORNER_Y2 660

//pacman
#define PACMAN_SPEED 5
#define BLANK_TILE 5
#define DOT_TILE 0
#define GALAXY_RIGHT_TILE 4
#define GALAXY_LEFT_TILE 3
#define HUNTER_MODE_TILE 6
#define WALL_TILE 1

//ghost
#define GHOST_SPEED 3
#define GHOST_DOOR_TILE 2
#define ERROR_NUMBER_OF_GHOST 20

//fps
#define FPS 35

//le ben trai
#define SIDE_LEFT 100

//menu
#define PAC_EAT_GHOST_SCORE 100



typedef struct Map
{
	//vi tri cua o
	int start_x_;
	int start_y_;

	//chi so cua o
	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name;
};

//trang thai di chuyen cua pacman
typedef struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;


};

typedef struct Node {
	int x, y;
	Node* parent;
};

//map01
static int map01[] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
1, 6, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 6, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1,
1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1,
1, 1, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 1, 1,
3, 0, 0, 0, 0, 0, 0, 1, 5, 5, 5, 1, 0, 0, 0, 0, 0, 0, 4,
1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
1, 1, 1, 1, 0, 1, 0, 5, 5, 5, 5, 5, 0, 1, 0, 1, 1, 1, 1,
1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
1, 6, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 6, 1,
1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1,
1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

#endif