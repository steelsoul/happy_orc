/*
 * CRectangle.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: alexander
 */

#include "CRectangle.hpp"

namespace framework {
namespace math {

CRectangle::CRectangle(double x, double y, double w, double h)
: mLowerLeft(x,y)
, mwidth(w)
, mheight(h)
{
}

CRectangle::~CRectangle() {
}

} /* namespace math */
} /* namespace framework */
