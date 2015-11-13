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
	static const int DEFAULT_TIMER_ID = 0;
public:
	CBaseScreen(CMainDispatcher& dispatcher, TTF_Font* font);

	virtual ~CBaseScreen();

	/**
	 * IPlayable interface
	 * */
	void init(SDL_Renderer* renderer) override;

	bool run(SDL_Window* window, SDL_Renderer* renderer) override;

	void onPrepare(int perc) override;

	bool isAlive() const override {
		return mAlive;
	}

	void cleanup(IPlayable* playable) override;

public:
	bool getEvent(int event) const;
	bool isTimerRunning() const; 
	void resetTimer();
	void quit();

protected:
	void processInputEvents(int delayMs);

	virtual void drawFrame(SDL_Window* window, SDL_Renderer* renderer);

	void setAlive(bool value) {
		mAlive = value;
	}

	void sendOnComplete();

	/**
		Rendering text from strings array line by line with baseColor colored.
		If selection is in range [0-count-1], then changes selected item color to selectionColor. 
		Returns: SDL_Surface* with text data.
	*/
	SDL_Surface* drawMenuTextBlock(SDL_Renderer* renderer, const char** strings, const int count, int selection = 0,
		SDL_Color baseColor = { 0xff, 0xaa, 0x00, 0 }, SDL_Color selectionColor = { 0xff, 0xff, 0xff, 0 });

private:
	virtual void onKeyDown(const SDL_Event& event);
	virtual void onKeyUp(const SDL_Event& event);

private:
	CMainDispatcher& mDispatcher;
	bool mAlive = false;

	std::map<int, bool> mInputEvents;
	TTF_Font* mFont = nullptr;
	int mTimerID = DEFAULT_TIMER_ID;
};

#endif /* SRC_CBASESCREEN_HPP_ */
