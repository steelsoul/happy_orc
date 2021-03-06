/*
 * ICompleteListener.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#ifndef ICOMPLETELISTENER_HPP_
#define ICOMPLETELISTENER_HPP_

class IPlayable;

class ICompleteListener {
public:
	virtual ~ICompleteListener() {}
	virtual void onPrepare(int percentage) = 0;
	virtual void onComplete(IPlayable*) = 0;
};

#endif /* ICOMPLETELISTENER_HPP_ */
