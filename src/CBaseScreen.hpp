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
	static const char* DEFAULT_FONT_NAME;
	static const int DEFAULT_FONT_SIZE = 30;
	static const int DEFAULT_TIMER_ID = 0;
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
	bool isTimerRunning() const; 
	void resetTimer();
	void quit();

protected:
	void processInputEvents(int delayMs);
	bool initTTF(const char* const fontname = CBaseScreen::DEFAULT_FONT_NAME, 
				const int fontsize = CBaseScreen::DEFAULT_FONT_SIZE);
	void closeTTF();
	virtual void drawFrame();

	void setAlive(bool value) {
		mAlive = value;
	}

	void sendOnComplete();

	/**
		Rendering text from strings array line by line with baseColor colored.
		If selection is in range [0-count-1], then changes selected item color to selectionColor. 
		Modify: texture. If the memory was allocated for texture then it will be lost. 
	*/
	void drawMenuTextBlock(const char** strings, const int count, SDL_Texture*& texture, int selection = 0,
		SDL_Color baseColor = { 0xff, 0xaa, 0x00, 0 }, SDL_Color selectionColor = { 0xff, 0xff, 0xff, 0 });

	void changeTextSize(int newTextSize);

private:
	virtual void onKeyDown(const SDL_Event& event);
	virtual void onKeyUp(const SDL_Event& event);

protected:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

private:
	CMainDispatcher& mDispatcher;
	bool mAlive = false;

	std::map<int, bool> mInputEvents;
	TTF_Font* mFont = nullptr;
	const char* mFontName = DEFAULT_FONT_NAME;
	int mTimerID = DEFAULT_TIMER_ID;
	int mFontSize = DEFAULT_FONT_SIZE;
};

#endif /* SRC_CBASESCREEN_HPP_ */
