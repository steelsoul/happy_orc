/*
 * CHamSprite.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: alexander
 */

#include "CHamSprite.hpp"
#include "settings.hpp"

namespace happyorc {

CHamSprite::CHamSprite(int x, int y, int w, int h, uint32_t s,
		int dh)
: CBaseSprite(x, y, w, h, s)
, mDisplayHeight(dh)
{
}

CHamSprite::~CHamSprite()
{
}

void CHamSprite::moveDown()
{
	CBaseSprite::moveDown();
}

CRectangle CHamSprite::getSrcRect() const
{
	return CRectangle(0, 0, HAM_SPRITE_WIDTH, HAM_SPRITE_HEIGHT);
}

} /* namespace happyorc */
