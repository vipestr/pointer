#include "stdafx.h"
#include "MouseEvents.h"
#include <Windows.h>
#include <iostream>

int
MouseEvents::SCREEN_SIZE_X = 1;
int
MouseEvents::SCREEN_SIZE_Y = 1;

void
MouseEvents::mouseMove(const Point &pt)
{
	int x = static_cast<int>(pt.getX() * SCREEN_SIZE_X);
	int y = static_cast<int>(pt.getY() * SCREEN_SIZE_Y);
	BOOL res = SetCursorPos(x, y);
	if (!res) {
		unsigned long err = GetLastError();
		std::cerr << "SetCursorPos error: " << err << std::endl;
	}
	return;
}

void
MouseEvents::init()
{
	SCREEN_SIZE_X = GetSystemMetrics(SM_CXSCREEN);
	SCREEN_SIZE_Y = GetSystemMetrics(SM_CYSCREEN);
}

