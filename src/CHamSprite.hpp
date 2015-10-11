/*
 * CHamSprite.hpp
 *
 *  Created on: Oct 11, 2015
 *      Author: alexander
 */

#ifndef SRC_CHAMSPRITE_HPP_
#define SRC_CHAMSPRITE_HPP_

#include "CBaseSprite.hpp"

namespace happyorc {

class CHamSprite: public CBaseSprite {
public:
	explicit CHamSprite(int x, int y, int w, int h, uint32_t s,
			int dh);
	virtual ~CHamSprite();

	void moveDown() override;
	CRect getSrcRect() const override;

private:
	int mDisplayHeight;
};

} /* namespace happyorc */

#endif /* SRC_CHAMSPRITE_HPP_ */
