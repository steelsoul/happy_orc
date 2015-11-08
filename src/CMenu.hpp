/*
 * CMenu.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CMENU_HPP_
#define SRC_CMENU_HPP_

#include "SDL.h"
#include "SDL_ttf.h"
#include "IPlayable.hpp"

#include <map>


class CMainDispatcher;
class COptions;

class CMenu: public IPlayable {
public:
	CMenu(CMainDispatcher& );
	virtual ~CMenu();

	virtual void init(SDL_Window* window, SDL_Renderer* renderer) override;
	virtual bool run() override;
	virtual void onPrepare(int perc) override;
	virtual bool isAlive() const override;

	void quit();

private:
	void onKeyDown(const SDL_Event& event);
	void onKeyUp(const SDL_Event& event);

	void drawFrame();

	void processInput();

private:
	CMainDispatcher& mDispatcher;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
	std::map<int, int> mEvents;
	bool mAlive;
	int mSelection;
	SDL_Texture* mMenuTexture;
	bool mInputTimerEnable;
	int mTickCounter;

	COptions* mOptions;
};

#endif /* SRC_CMENU_HPP_ */
