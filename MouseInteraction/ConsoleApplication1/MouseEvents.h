#pragma once
#include "Point.h"

class MouseEvents
{
public:
	static void mouseMove(const Point &pt);
	static void init();
private:
	MouseEvents(){};
	~MouseEvents(){};
	static int SCREEN_SIZE_X;
	static int SCREEN_SIZE_Y;
};

