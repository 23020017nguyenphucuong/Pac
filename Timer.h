#pragma once
#ifndef TIMER_H
#define TIMER_H
#include "CommonFunc.h"

class Timer
{
private:
	int start_tick_;
	int paused_tick_;

	bool is_paused_;
	bool is_started_;

public:
	Timer();
	~Timer();

	void start();
	void stop();
	void paused();
	void resumed();

	int get_sticks();

	bool is_started();
	bool is_paused();
};



#endif