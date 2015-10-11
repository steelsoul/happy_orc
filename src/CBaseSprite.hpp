/*
 * CBaseSprite.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CBASESPRITE_HPP_
#define SRC_CBASESPRITE_HPP_

#include <cstdint>

namespace happyorc {

class CBaseSprite {
public:

	struct CRect {
		CRect(int x, int y, int w, int h)
		: x(x), y(y), w(w), h(h) {}
		int x, y, w, h;
	};

	explicit CBaseSprite(int x, int y, int w, int h, uint32_t s);
	virtual ~CBaseSprite();

	virtual void moveRight();
	virtual void moveLeft();
	virtual void moveUp();
	virtual void moveDown();

	void setX(int x) { mx = x; }
	void setY(int y) { my = y; }

	int getX() const {return mx; }
	int getY() const {return my; }
	int getW() const {return mw; }
	int getH() const {return mh; }
	uint32_t getSpeed() const { return ms; }

	void setSpeed(uint32_t speed);
	virtual CRect getSrcRect() const;
private:
	int mx, my, mw, mh;
	uint32_t ms;
};

} /* namespace happyorc */

#endif /* SRC_CBASESPRITE_HPP_ */
