/*
 * CSprite.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include "CSprite.hpp"

namespace happyorc {

CSprite::CSprite()
: CBaseSprite()
, mStates(0)
, mState(0)
, mWidth(0)
, mHeight(0)
, mSpeed(0)
, mDisplayWidth(0)
{
}

CSprite::CSprite(int states,
		int width,
		int height,
		int speed,
		int dw)
: CBaseSprite()
, mStates(states)
, mState(0)
, mWidth(width)
, mHeight(height)
, mSpeed(speed)
, mDisplayWidth(dw) {
}


CSprite::~CSprite() {

}

void CSprite::moveLeft() {
	x -= mSpeed;
	if (0 >= x) {
		x = 0;
	} else
	{
		update_left();
	}
}

void CSprite::moveRight() {
	x += mSpeed;
	if (mDisplayWidth <= x + mWidth - mSpeed) {
		x = mDisplayWidth - mWidth;
	} else
	{
		update_right();
	}
}

CSprite::CRect CSprite::getSrcRect() const {
	CRect result;
	result.x = mState * mWidth;
	result.y = 0;
	result.w = mWidth;
	result.h = mHeight;

	return result;
}

void CSprite::update_right() {
	if (x % 20 == 0) {
		mState++;
		if (mStates < mState) {
			mState = 0;
		}
	}
}

void CSprite::update_left() {
	if (x % 20 == 0) {
		mState--;
		if (0 > mState) {
			mState = mStates;
		}
	}
}


} /* namespace happyorc */
