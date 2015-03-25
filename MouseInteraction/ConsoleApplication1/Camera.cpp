#include "stdafx.h"
#include "Camera.h"

CameraOpenException::CameraOpenException()
{
}

CameraOpenException::~CameraOpenException()
{
}

const char *
CameraOpenException::what()
{
	return "Opening camera device failed";
}

Camera::Camera()
{
}


Camera::~Camera()
{
}

void
Camera::init() {
	videoCapture.open(1);
	if (!videoCapture.isOpened()) {
		throw CameraOpenException();
	}
}

Point
Camera::getPosition()
{
	cv::Mat img;
	videoCapture >> img;
	char c = cv::waitKey(1);
	return RectangleDetection::getPosition(img);
}
