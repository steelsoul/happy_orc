/*
 * COrcSprite.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CORCSPRITE_HPP_
#define SRC_CORCSPRITE_HPP_

#include "CBaseSprite.hpp"

namespace happyorc {

class COrcSprite: public CBaseSprite {
public:
	explicit COrcSprite(int x, int y, int w, int h, uint32_t s,
			int dw, int states, int defaultstate = 0);
	virtual ~COrcSprite();

	void moveRight(double deltaTime) override;
	void moveLeft(double deltaTime) override;

	CRectangle getSrcRect() const override;
protected:
	void update_left();
	void update_right();
private:
	int mStates;
	int mState;
	int mDisplayWidth;
};

} /* namespace happyorc */

#endif /* SRC_CORCSPRITE_HPP_ */
