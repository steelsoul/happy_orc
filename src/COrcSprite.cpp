/*
 * COrcSprite.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include "COrcSprite.hpp"
#include "settings.hpp"

#include <cstdio>

namespace happyorc {

COrcSprite::COrcSprite(int x, int y, int w, int h, uint32_t s,
		int dw, int states, int defaultState)
: CBaseSprite(x, y, w, h, s)
, mStates(states)
, mState(defaultState)
, mDisplayWidth(dw)
, mStateCounter(0u)
{
}

COrcSprite::~COrcSprite()
{
}

void COrcSprite::moveRight(double deltaTime)
{
	CBaseSprite::moveRight(deltaTime);
	if (mDisplayWidth <= getX() + getW() - getSpeed())
	{
		setX(mDisplayWidth - getW());
	}
	else
	{
		update_right();
	}
}

void COrcSprite::moveLeft(double deltaTime)
{
	CBaseSprite::moveLeft(deltaTime);
	if (0 >= getX())
	{
		setX(0);
	}
	else
	{
		update_left();
	}

}

void COrcSprite::update_right()
{
	mStateCounter++;
	if (mStateCounter % 20 == 0)
	{
		mState++;
		if (mState > mStates)
		{
			mState = 0;
		}
	}
	if (mStateCounter > 100u) mStateCounter = 0u;
}

void COrcSprite::update_left()
{
	mStateCounter++;
	if (mStateCounter % 20 == 0)
	{
		mState--;
		if (mState < 0)
		{
			mState = mStates;
		}
	}
	if (mStateCounter > 100u) mStateCounter = 0u;
}


CRectangle COrcSprite::getSrcRect() const
{
	return CRectangle(mState * ORC_SPRITE_WIDTH, 0,
			ORC_SPRITE_WIDTH, ORC_SPRITE_HEIGHT);
}




} /* namespace happyorc */
