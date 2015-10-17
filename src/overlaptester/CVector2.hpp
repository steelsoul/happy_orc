/*
 * CVector2.hpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#ifndef SRC_OVERLAPTESTER_CVECTOR2_HPP_
#define SRC_OVERLAPTESTER_CVECTOR2_HPP_

#include <SDL_stdinc.h>

namespace framework {
namespace math {

const double TO_RADIANS = (1 / 180.0) * M_PI;
const double TO_DEGREES = (1 / M_PI) * 180.0;

class CVector2 {
public:
	CVector2();
	CVector2(double x, double y);

	CVector2& set(double x, double y);
	CVector2& add(double x, double y);
	CVector2& add(const CVector2& other);
	CVector2& sub(double x, double y);
	CVector2& sub(const CVector2& other);
	CVector2& mul(const double scalar);

	double len() const;
	CVector2& nor();
	double angle() const;
	CVector2& rotate(double angle);
	double dist(const CVector2& other) const ;
	double dist(double x, double y) const;
	double dist2(const CVector2& other) const;
	double dist2(double x, double y) const;

	virtual ~CVector2();
public:
	double mx, my;
};

} /* namespace math */
} /* namespace framework */

#endif /* SRC_OVERLAPTESTER_CVECTOR2_HPP_ */
