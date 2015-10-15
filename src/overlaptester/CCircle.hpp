/*
 * CCIrcle.hpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#ifndef SRC_OVERLAPTESTER_CCIRCLE_HPP_
#define SRC_OVERLAPTESTER_CCIRCLE_HPP_

#include "CVector2.hpp"

namespace framework {
namespace math {

class CCircle {
public:
	CCircle(double x, double y, double radius);
	virtual ~CCircle();

	CVector2 mcenter;
	double mradius;
};

} /* namespace math */
} /* namespace framework */

#endif /* SRC_OVERLAPTESTER_CCIRCLE_HPP_ */
