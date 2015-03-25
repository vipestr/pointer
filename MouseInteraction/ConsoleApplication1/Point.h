#pragma once
class Point
{
public:
	Point();
	Point(const double &x, const double &y);
	~Point();
	double getX() const;
	double getY() const;
	void setX(const double &x);
	void setY(const double &y);
private:
	double x, y;
};

