/*
 * IPlayable.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#ifndef SRC_IPLAYABLE_HPP_
#define SRC_IPLAYABLE_HPP_

#include <SDL.h>

class IPlayable {
public:
	virtual ~IPlayable() {}
	virtual void init(SDL_Window* window, SDL_Renderer* renderer) = 0;
	virtual bool run() = 0;
	virtual void onPrepare(int perc) = 0;
	virtual bool isAlive() const = 0;
};


#endif /* SRC_IPLAYABLE_HPP_ */
