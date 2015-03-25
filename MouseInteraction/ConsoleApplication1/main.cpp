// main.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Camera.h"
#include "MouseEvents.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	Camera cam;
	try {
		cam.init();
	}
	catch (CameraOpenException e) {
		std::cerr << e.what() << std::endl;
		return 0;
	}
	MouseEvents::init();

	while (true) {
		Point pt = cam.getPosition();
				MouseEvents::mouseMove(pt);
	}
	return 0;
}

