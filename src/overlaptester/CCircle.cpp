/*
 * CCIrcle.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#include "CCircle.hpp"

namespace framework {
namespace math {

CCircle::CCircle(double x, double y, double radius)
: mcenter(x,y)
, mradius(radius)
{
}

CCircle::~CCircle() {
}

} /* namespace math */
} /* namespace framework */
