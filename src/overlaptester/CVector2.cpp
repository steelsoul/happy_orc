/*
 * CVector2.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#include "CVector2.hpp"

namespace framework {
namespace math {

CVector2::CVector2()
: mx(0)
, my(0)
{
}

CVector2::CVector2(double x, double y)
: mx(x)
, my(y)
{
}

CVector2::~CVector2()
{
}

CVector2& CVector2::set(double x, double y)
{
	mx = x;
	my = y;
	return *this;
}

CVector2& CVector2::add(double x, double y)
{
	return add(CVector2(x,y));
}

CVector2& CVector2::add(const CVector2& other)
{
	mx += other.mx;
	my += other.my;
	return *this;
}

CVector2& CVector2::sub(double x, double y)
{
	return sub(CVector2(x,y));
}

CVector2& CVector2::sub(const CVector2& other)
{
	mx -= other.mx;
	my -= other.my;
	return *this;
}

CVector2& CVector2::mul(const double scalar)
{
	mx *= scalar;
	my *= scalar;
	return *this;
}

double CVector2::len() const
{
	return /*std::*/SDL_sqrt(mx*mx + my*my);
}

CVector2& CVector2::nor()
{
	double alen = len();
	if (alen != 0.0) {
		mx /= alen;
		my /= alen;
	}
	return *this;
}

double CVector2::angle() const
{
	double angle = SDL_atan2(my, mx) * TO_DEGREES;
	if (angle < 0) angle += 360;
	return angle;
}

CVector2& CVector2::rotate(double angle)
{
	double rad = angle * TO_RADIANS;
	double acos = /*std::*/SDL_cos(rad);
	double asin = /*std::*/SDL_sin(rad);

	double newX = mx * acos - my * asin;
	double newY = mx * asin + my * acos;

	mx = newX;
	my = newY;

	return *this;
}

double CVector2::dist(const CVector2& other) const
{
	double distX = mx - other.mx;
	double distY = my - other.my;
	return /*std::*/SDL_sqrt(distX * distX + distY * distY);
}

double CVector2::dist(double x, double y) const
{
	return dist(CVector2(x,y));
}

double CVector2::dist2(const CVector2& other) const
{
	double distX = mx - other.mx;
	double distY = my - other.my;
	return distX * distX + distY * distY;
}

double CVector2::dist2(double x, double y) const
{
	return dist2(CVector2(x,y));
}



} /* namespace math */
} /* namespace framework */
