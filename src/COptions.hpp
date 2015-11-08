/*
 * COptions.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#ifndef SRC_COPTIONS_HPP_
#define SRC_COPTIONS_HPP_

#include "IPlayable.hpp"
#include "SDL_ttf.h"

class CMainDispatcher;

class COptions: public IPlayable {
public:
	COptions(CMainDispatcher& );
	virtual ~COptions();

	/**
	 * IPlayable interface
	 * */

	virtual void init(SDL_Window* window, SDL_Renderer* renderer) override;
	virtual bool run() override;
	virtual void onPrepare(int perc) override;
	virtual bool isAlive() const override;

private:
	void drawFrame();

private:
	CMainDispatcher& mDispatcher;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	TTF_Font* mFont;
	bool mAlive;
};

#endif /* SRC_COPTIONS_HPP_ */
