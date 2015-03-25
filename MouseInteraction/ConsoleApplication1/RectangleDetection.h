#pragma once
#include <vector>
#include <opencv2\highgui\highgui.hpp>
#include "Point.h"

class RectangleDetection
{
public:
	static Point getPosition(cv::Mat img);
private:
	RectangleDetection();
	~RectangleDetection();
};

