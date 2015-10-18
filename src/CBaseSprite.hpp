/*
 * CBaseSprite.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CBASESPRITE_HPP_
#define SRC_CBASESPRITE_HPP_

#include <cstdint>
#include "overlaptester/CRectangle.hpp"

namespace happyorc {

using framework::math::CRectangle;

class CBaseSprite {
public:
	
	explicit CBaseSprite(int x, int y, int w, int h, uint32_t s);
	virtual ~CBaseSprite();

	virtual void moveRight(double deltaTime);
	virtual void moveLeft(double deltaTime);
	virtual void moveUp(double deltaTime);
	virtual void moveDown(double deltaTime);

	void setX(int x);
	void setY(int y);

	int getX() const {return mx; }
	int getY() const {return my; }
	int getW() const {return mw; }
	int getH() const {return mh; }
	uint32_t getSpeed() const { return ms; }

	void setSpeed(uint32_t speed);
	virtual CRectangle getSrcRect() const;
	virtual CRectangle getBound() const;
private:
	double mx, my, mw, mh;
	CRectangle mBound;
	uint32_t ms;
};

} /* namespace happyorc */

#endif /* SRC_CBASESPRITE_HPP_ */
