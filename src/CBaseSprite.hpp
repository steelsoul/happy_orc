/*
 * CBaseSprite.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CBASESPRITE_HPP_
#define SRC_CBASESPRITE_HPP_

#include "settings.hpp"

namespace happyorc {

class CBaseSprite {
public:

	CBaseSprite();
	virtual void moveLeft();
	virtual void moveRight();
	virtual ~CBaseSprite();

	int x, y;
};

} /* namespace happyorc */

#endif /* SRC_CBASESPRITE_HPP_ */
