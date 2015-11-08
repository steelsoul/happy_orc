/*
 * IDestroyListener.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#ifndef SRC_IDESTROYLISTENER_HPP_
#define SRC_IDESTROYLISTENER_HPP_

class IPlayable;

class IDestroyListener {
public:
	virtual ~IDestroyListener() {}
	virtual void onDestroy(IPlayable* which) = 0;
};



#endif /* SRC_IDESTROYLISTENER_HPP_ */
