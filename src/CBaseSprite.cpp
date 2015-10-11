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
{
}

CBaseSprite::~CBaseSprite() {

}

void CBaseSprite::setSpeed(uint32_t speed)
{
	ms = speed;
}

void CBaseSprite::moveLeft()
{
	mx -= ms;
}

void CBaseSprite::moveRight()
{
	mx += ms;
}

void CBaseSprite::moveDown()
{
	my += ms;
}

void CBaseSprite::moveUp()
{
	my -= ms;
}

CBaseSprite::CRect CBaseSprite::getSrcRect() const
{
	return CRect(mx, my, mw, mh);
}

} /* namespace happyorc */
