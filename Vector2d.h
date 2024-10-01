#pragma once

#include <cmath>


struct Vector2d
{
	double x_;
	double y_;

	Vector2d(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}
	Vector2d operator+(Vector2d const& v)
	{
		return Vector2d(x_ + v.x_, y_ + v.y_);
	}
	Vector2d operator-(Vector2d const& v)
	{
		return Vector2d(x_ - v.x_, y_ - v.y_);
	}
	Vector2d& operator=(Vector2d const& v)
	{
		if (this != &v)
		{
			x_ = v.x_;
			y_ = v.y_;
		}
		return *this;
	}
	Vector2d& operator/(double const& d)
	{
		if (d != 0)
		{
			x_ /= d;
			y_ /= d;
		}
		return *this;
	}

	double len() { return sqrt(x_ * x_ + y_ * y_); }

	Vector2d& operator*(double const& d)
	{
		x_ *= d;
		y_ *= d;
		return *this;
	}

	Vector2d direction()
	{
		double l = len();
		if (l != 0) return *this / l;
		return *this;
	}
};