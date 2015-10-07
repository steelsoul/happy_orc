/*
 * CSprite.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CSPRITE_HPP_
#define SRC_CSPRITE_HPP_

#include "CBaseSprite.hpp"

namespace happyorc {

class CSprite: public CBaseSprite {
public:
	struct CRect {
		int x, y, w, h;
	};

	CSprite();
	CSprite(int states, int width, int height, int speed, int dw);
	virtual ~CSprite();

	void moveLeft();
	void moveRight();

	CRect getSrcRect() const;
protected:
	void update_left();
	void update_right();
private:
	int mStates, mState, mWidth, mHeight, mSpeed;
	int mDisplayWidth;
};

} /* namespace happyorc */

#endif /* SRC_CSPRITE_HPP_ */
