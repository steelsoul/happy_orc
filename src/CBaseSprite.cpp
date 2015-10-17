/*
 * CBaseSprite.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include "CBaseSprite.hpp"

namespace happyorc {

CBaseSprite::CBaseSprite(int x, int y, int w, int h, uint32_t s)
: mx(x)
, my(y)
, mw(w)
, mh(h)
, ms(s)
, mBound(x,y,w,h)
{
}

CBaseSprite::~CBaseSprite() {

}

void CBaseSprite::setSpeed(uint32_t speed)
{
	ms = speed;
}

void CBaseSprite::setX(int x)
{
	mx = x;
	mBound.mLowerLeft.mx = mx;
}

void CBaseSprite::setY(int y)
{
	my = y;
	mBound.mLowerLeft.my = my;
}

void CBaseSprite::moveLeft()
{
	mx -= ms;
	mBound.mLowerLeft.mx = mx;
}

void CBaseSprite::moveRight()
{
	mx += ms;
	mBound.mLowerLeft.mx = mx;
}

void CBaseSprite::moveDown()
{
	my += ms;
	mBound.mLowerLeft.my = my;
}

void CBaseSprite::moveUp()
{
	my -= ms;
	mBound.mLowerLeft.my = my;
}

CRectangle CBaseSprite::getSrcRect() const
{
	return CRectangle(mx, my, mw, mh);
}

CRectangle CBaseSprite::getBound() const
{
	return mBound;
}

} /* namespace happyorc */
