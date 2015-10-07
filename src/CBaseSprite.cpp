/*
 * CBaseSprite.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include "CBaseSprite.hpp"

namespace happyorc {

CBaseSprite::CBaseSprite()
: x(0)
, y(0)
{

}

CBaseSprite::~CBaseSprite() {

}

void CBaseSprite::moveLeft() {
	x -= HERO_SPEED;
}

void CBaseSprite::moveRight() {
	x += HERO_SPEED;
}

} /* namespace happyorc */
