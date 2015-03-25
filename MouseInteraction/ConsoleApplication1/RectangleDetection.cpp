#include "stdafx.h"
#include "RectangleDetection.h"
#include <iostream>
#include <algorithm>
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\calib3d\calib3d.hpp"
#include "opencv2\features2d\features2d.hpp"

const int OPENING_SIZE = 3;
std::vector<cv::Point2f> previousRectangle;
Point previousPoint;

RectangleDetection::RectangleDetection()
{
}


RectangleDetection::~RectangleDetection()
{
}


static void drawSquares(cv::Mat image, const std::vector<std::vector<cv::Point> >& squares)
{
	for (size_t i = 0; i < squares.size(); i++)
	{
		const cv::Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		cv::polylines(image, &p, &n, 1, true, cv::Scalar(100), 3, CV_AA);
	}
	imshow("rect", image);
}

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


std::vector<cv::Point2f> detectRectangle(cv::Mat img) {
	std::vector<std::vector<cv::Point> > squares;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Point> approx;

	double area = img.size().area();


	cv::findContours(img.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i) {
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

		if (approx.size() == 4 &&
			std::abs(cv::contourArea(cv::Mat(approx))) > area * 0.1 &&
			std::abs(cv::contourArea(cv::Mat(approx))) < area * 0.9 &&
			cv::isContourConvex(cv::Mat(approx))) {

			double maxCosine = 0;
			for (int j = 2; j < 5; ++j) {
				double cosine = std::abs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = std::max(maxCosine, cosine);
			}
			if (maxCosine < 0.3) {
				squares.push_back(approx);
			}
		}
	}

	drawSquares(img, squares);
	std::vector<cv::Point2f> ret;
	if (squares.empty()) {
		return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret.push_back(cv::Point2f(squares.front()[i].x, squares.front()[i].y));
	}
	return ret;
}


Point
getPointer(cv::Mat img) {
	cv::Mat mouse;
	std::vector<cv::KeyPoint> keypoints;
	cv::SimpleBlobDetector::Params params;

	cv::cvtColor(img.clone(), img, CV_BGR2GRAY);

	params.filterByColor = true;
	params.blobColor = 255;

	// Change thresholds
	params.minThreshold = 200;
	params.maxThreshold = 255;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 0;
	params.maxArea = 300;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.2;
	params.maxCircularity = 1;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.7;
	params.maxConvexity = 1;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.2;
	params.maxInertiaRatio = 1;


	cv::FeatureDetector *detector = new cv::SimpleBlobDetector(params);
	detector->detect(img, keypoints);
	std::cout << "Detector detected:" << keypoints.size() << " keypoints" << std::endl;
	cv::drawKeypoints(img, keypoints, mouse, cv::Scalar(0, 0, 0));
	cv::imshow("mouse", mouse);

	delete detector;

	if (keypoints.empty()) {
		return previousPoint;
	}
	Point currentPoint = Point(keypoints[0].pt.x / img.size().width, keypoints[0].pt.y / img.size().height);
	previousPoint = currentPoint;
	return currentPoint;
}

Point
RectangleDetection::getPosition(cv::Mat img)
{
	int height = img.size().height;
	int width = img.size().width;


	std::vector<cv::Point2f> screenCorners = { cv::Point2f(0, 0), cv::Point2f(width, 0), cv::Point2f(width, height), cv::Point2f(0, height)};
	
	if (previousRectangle.empty()) {
		previousRectangle = screenCorners;
	}

	cv::Mat screen;

	cv::Mat res, gray;
	std::vector<cv::Point2f> rectCorners;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * OPENING_SIZE + 1, 2 * OPENING_SIZE + 1), cv::Point(OPENING_SIZE, OPENING_SIZE));

	cv::cvtColor(img, gray, CV_BGR2GRAY);
	cv::adaptiveThreshold(gray, res, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 91, 0);

	cv::morphologyEx(res.clone(), res, cv::MORPH_OPEN, element);

	rectCorners = detectRectangle(res);

	if (rectCorners.empty()) {
		std::cerr << "Not detected!!!!" << std::endl;
		rectCorners = previousRectangle;
	}

	previousRectangle = rectCorners;


	cv::Mat H = cv::findHomography(rectCorners, screenCorners);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cerr << H.at<double>(i, j) << " ";
		}
		std::cerr << std::endl;
	}
	cv::warpPerspective(img, screen, H, img.size());
	cv::imshow("result", screen);
	return getPointer(screen);
}