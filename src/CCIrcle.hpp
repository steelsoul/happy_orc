/*
 * CCIrcle.hpp
 *
 *  Created on: Oct 14, 2015
 *      Author: alexander
 */

#ifndef SRC_CCIRCLE_HPP_
#define SRC_CCIRCLE_HPP_

namespace framework {
namespace math {

class CCIrcle {
public:
	CCIrcle();
	virtual ~CCIrcle();

private:
	CVector2 center;
};

} /* namespace math */
} /* namespace framework */

#endif /* SRC_CCIRCLE_HPP_ */
