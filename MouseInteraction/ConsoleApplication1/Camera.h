#pragma once
#include <opencv2\highgui\highgui.hpp>
#include "Exception.h"
#include "RectangleDetection.h"
#include "Point.h"

class CameraOpenException : public Exception {
public:
	CameraOpenException();
	~CameraOpenException();
	virtual const char *what();
};

class Camera
{
public:
	Camera();
	~Camera();
	void init();
	Point getPosition();
private:
	cv::VideoCapture videoCapture;
};

