/*
 * CBaseScreen.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: alexander
 */

#include "CBaseScreen.hpp"
#include "CMainDispatcher.hpp"

Uint32 my_callbackfn(Uint32 interval, void* param)
{
	if (param) {
		CBaseScreen* m = reinterpret_cast<CBaseScreen*>(param);
		if (m) {
			m->resetTimer();
		}
	}
	return interval;
}

CBaseScreen::CBaseScreen(CMainDispatcher& dispatcher)
: mDispatcher(dispatcher)
{
}

CBaseScreen::~CBaseScreen()
{
	if (mTimerID) {
		SDL_RemoveTimer(mTimerID);
	}
	closeTTF();
}

void CBaseScreen::init(SDL_Window* window, SDL_Renderer* renderer)
{
	mWindow = window;
	mRenderer = renderer;

	mAlive = initTTF();
	mDispatcher.onComplete();
}

bool CBaseScreen::run()
{
	if (mAlive) {
		processInputEvents(100);
		drawFrame();
	}
}

void CBaseScreen::onPrepare(int perc)
{
}

void CBaseScreen::cleanup()
{
}

bool CBaseScreen::getEvent(int event) const
{
	if (mInputEvents.find(event) != mInputEvents.end()) {
		return mInputEvents.at(event);
	} else {
		return false;
	}
}

void CBaseScreen::resetTimer() {
	mTimerID = 0;
}

void CBaseScreen::processInputEvents(int delayMs)
{
	SDL_Event event ;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mDispatcher.onDestroy(this);
			mAlive = false;
			break;
		case SDL_KEYDOWN:
			if (mTimerID == 0) {
				mTimerID = SDL_AddTimer(delayMs, &my_callbackfn, this);
				onKeyDown(event);
			}
			break;
		case SDL_KEYUP:
			if (mTimerID == 0) {
				mTimerID = SDL_AddTimer(delayMs, &my_callbackfn, this);
				onKeyUp(event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
			break;
		}
	}
}

bool CBaseScreen::initTTF(const char* const fontname, const int fontsize)
{
	/* Initialize the TTF library */
	bool ttfInitResult = (TTF_Init() == 0);

	if (ttfInitResult) {
		mFont = TTF_OpenFont(fontname, fontsize);
		if (!mFont) {
			ttfInitResult = false;
		}
	}

	if (ttfInitResult) {
		TTF_SetFontStyle(mFont, TTF_STYLE_NORMAL | TTF_STYLE_BOLD);
		TTF_SetFontOutline(mFont, 0);
		TTF_SetFontKerning(mFont, 0);
		TTF_SetFontHinting(mFont, TTF_HINTING_NORMAL);
	}

	return ttfInitResult;
}

void CBaseScreen::closeTTF()
{
	TTF_CloseFont(mFont);
	TTF_Quit();
}

void CBaseScreen::drawFrame()
{
	// Clear screen
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);
}

void CBaseScreen::sendOnComplete() {
	mDispatcher.onComplete();
}

void CBaseScreen::onKeyDown(const SDL_Event& event)
{
    mInputEvents[event.key.keysym.sym] = true;
}

void CBaseScreen::onKeyUp(const SDL_Event& event)
{
    mInputEvents[event.key.keysym.sym] = false;
}
