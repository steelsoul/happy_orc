/*
 * CMenu.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CMENUSCREEN_HPP_
#define SRC_CMENUSCREEN_HPP_

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_timer.h"
#include "IPlayable.hpp"

#include "CBaseScreen.hpp"

#include <map>


class CMainDispatcher;
class COptionsScreen;
class CGameScreen;

class CMenuScreen: public CBaseScreen {
public:
	CMenuScreen(CMainDispatcher&, TTF_Font* );
	virtual ~CMenuScreen();

	void init(SDL_Renderer* renderer) override;
	bool run(SDL_Window* window, SDL_Renderer* renderer, double dt) override;
	void onPrepare(int perc) override;
	void cleanup(IPlayable* playable) override;

protected:
	void onKeyDown(const SDL_Event& event);
	void onKeyUp(const SDL_Event& event);
	void drawFrame(SDL_Window* window, SDL_Renderer* renderer);

private:
	void processInput();

private:
	CMainDispatcher& mDispatcher;
	TTF_Font* mFont;
	int mSelection;
	SDL_Texture* mMenuTexture;
	bool mChanged = true;

	COptionsScreen* mOptions;
	CGameScreen* mGame;
};

#endif /* SRC_CMENUSCREEN_HPP_ */
