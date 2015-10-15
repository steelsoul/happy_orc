/*
 * CRectangle.hpp
 *
 *  Created on: Oct 15, 2015
 *      Author: alexander
 */

#ifndef SRC_OVERLAPTESTER_CRECTANGLE_HPP_
#define SRC_OVERLAPTESTER_CRECTANGLE_HPP_

#include "CVector2.hpp"

namespace framework {
namespace math {

class CRectangle {
public:
	CRectangle(double x, double y, double w, double h);
	virtual ~CRectangle();

	CVector2 mLowerLeft;
	double mwidth, mheight;

};

} /* namespace math */
} /* namespace framework */

#endif /* SRC_OVERLAPTESTER_CRECTANGLE_HPP_ */
