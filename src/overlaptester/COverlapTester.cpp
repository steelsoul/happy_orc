/*
 * COverlapTester.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#include "COverlapTester.hpp"

namespace framework {
namespace math {

bool COverlapTester::overlapCircles(const CCircle& c1, const CCircle& c2)
{
	double distance = c1.mcenter.dist2(c2.mcenter);
	double radiusSum = c1.mradius + c2.mradius;
	return distance <= radiusSum * radiusSum;
}

bool COverlapTester::overlapRectangles(const CRectangle& r1, const CRectangle& r2)
{
	if (r1.mLowerLeft.mx < r2.mLowerLeft.mx + r2.mwidth &&
		r1.mLowerLeft.mx + r1.mwidth > r2.mLowerLeft.mx &&
		r1.mLowerLeft.my < r2.mLowerLeft.my + r2.mheight &&
		r1.mLowerLeft.my + r1.mheight > r2.mLowerLeft.my)
		return true;
	else
		return false;
}

bool COverlapTester::overlapCircleRectangle(const CCircle& c, const CRectangle& r)
{
	double closestX = c.mcenter.mx;
	double closestY = c.mcenter.my;

	if (c.mcenter.mx < r.mLowerLeft.mx) {
		closestX = r.mLowerLeft.mx;
	} else if (c.mcenter.mx > r.mLowerLeft.mx + r.mwidth) {
		closestX = r.mLowerLeft.mx + r.mwidth;
	}

	if (c.mcenter.my < r.mLowerLeft.my) {
		closestY = r.mLowerLeft.my;
	} else if (c.mcenter.my > r.mLowerLeft.my + r.mheight) {
		closestY = r.mLowerLeft.my + r.mheight;
	}

	return c.mcenter.dist2(closestX, closestY) < c.mradius * c.mradius;
}

bool COverlapTester::pointInCircle(const CVector2& p, const CCircle& c)
{
	return c.mcenter.dist2(p) < c.mradius * c.mradius;
}

bool COverlapTester::pointInRectangle(const CVector2& p, const CRectangle& r)
{
	return  r.mLowerLeft.mx <= p.mx && r.mLowerLeft.mx + r.mwidth >= p.mx &&
			r.mLowerLeft.my <= p.my && r.mLowerLeft.my + r.mheight >= p.my;
}

} /* namespace math */
} /* namespace framework */
