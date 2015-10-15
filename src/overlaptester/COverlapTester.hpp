/*
 * COverlapTester.hpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#ifndef SRC_OVERLAPTESTER_COVERLAPTESTER_HPP_
#define SRC_OVERLAPTESTER_COVERLAPTESTER_HPP_

#include "CVector2.hpp"
#include "CRectangle.hpp"
#include "CCircle.hpp"

namespace framework {
namespace math {

class COverlapTester {
public:
	static bool overlapCircles(const CCircle&, const CCircle&);
	static bool overlapRectangles(const CRectangle& r1, const CRectangle& r2);
	static bool overlapCircleRectangle(const CCircle& c, const CRectangle& r);
	static bool pointInCircle(const CVector2& p, const CCircle& c);
	static bool pointInRectangle(const CVector2& p, const CRectangle& r);
private:
	COverlapTester();
	COverlapTester(const COverlapTester&);
	COverlapTester& operator=(const COverlapTester&);
	~COverlapTester();
};
} /* namespace math */
} /* namespace framework */

#endif /* SRC_OVERLAPTESTER_COVERLAPTESTER_HPP_ */
