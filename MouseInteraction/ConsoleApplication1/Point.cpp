#include "stdafx.h"
#include "Point.h"


Point::Point()
{
}

Point::Point(const double &x, const double &y)
	: x(x), y(y)
{
}

Point::~Point()
{
}

void
Point::setX(const double &x)
{
	this->x = x;
}

void
Point::setY(const double &y)
{
	this->y = y;
}

double
Point::getX() const
{
	return x;
}

double
Point::getY() const
{
	return y;
}