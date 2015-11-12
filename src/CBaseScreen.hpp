/*
 * CBaseScreen.hpp
 *
 *  Created on: Nov 12, 2015
 *      Author: alexander
 */

#ifndef SRC_CBASESCREEN_HPP_
#define SRC_CBASESCREEN_HPP_

#include "IPlayable.hpp"

#include "SDL_ttf.h"

#include <map>

class CMainDispatcher;

class CBaseScreen: public IPlayable {
public:
	CBaseScreen(CMainDispatcher& dispatcher);

	virtual ~CBaseScreen();

	/**
	 * IPlayable interface
	 * */
	void init(SDL_Window* window, SDL_Renderer* renderer) override;

	bool run() override;

	void onPrepare(int perc) override;

	bool isAlive() const override {
		return mAlive;
	}

	void cleanup() override;

public:
	bool getEvent(int event) const;

	void resetTimer();

protected:
	void processInputEvents(int delayMs);
	bool initTTF(const char* const fontname = "Effinground.ttf", const int fontsize = 20);
	void closeTTF();
	virtual void drawFrame();

	void setAlive(bool value) {
		mAlive = value;
	}

	void sendOnComplete();

	//void drawTextBlock(const char** );

private:
	void onKeyDown(const SDL_Event& event);
	void onKeyUp(const SDL_Event& event);

private:
	CMainDispatcher& mDispatcher;
	bool mAlive = false;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	std::map<int, bool> mInputEvents;
	TTF_Font* mFont = nullptr;
	int mTimerID = 0;
};

#endif /* SRC_CBASESCREEN_HPP_ */
