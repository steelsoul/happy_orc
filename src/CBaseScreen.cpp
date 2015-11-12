/*
 * CBaseScreen.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: alexander
 */

#include "CBaseScreen.hpp"
#include "CMainDispatcher.hpp"

#include <vector>
#include <iostream>

using namespace std;

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

const char* CBaseScreen::DEFAULT_FONT_NAME = "Effinground.ttf";

CBaseScreen::CBaseScreen(CMainDispatcher& dispatcher)
: mDispatcher(dispatcher)
{
	cout << __FUNCTION__ << " [ctor]\n";
}

CBaseScreen::~CBaseScreen()
{
	cout << __FUNCTION__ << " [dtor]\n";
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
	return mAlive;
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

bool CBaseScreen::isTimerRunning() const
{
	return mTimerID != 0;
}

void CBaseScreen::resetTimer() {
	mTimerID = 0;
}

void CBaseScreen::quit() {
	mDispatcher.onDestroy(this);
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

		mFontName = fontname;
		mFontSize = fontsize;
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

void CBaseScreen::drawMenuTextBlock(const char** strings, const int count, SDL_Texture*& texture, int selection,
									SDL_Color baseColor, SDL_Color selectionColor)
{
	vector<SDL_Surface*> surfaces;	
	vector<SDL_Rect> sourcesRect;
	int totalW = 0; int totalH = 0;

	for (auto i = 0; i < count; i++) {
		SDL_Color color;
		SDL_Rect srcRect = {0, 0, 0, 0};


		color = (i == selection) ? selectionColor : baseColor;
		SDL_Surface* srcSurface = TTF_RenderText_Solid(mFont, strings[i], color);
		srcRect.w = srcSurface->w;
		srcRect.h = srcSurface->h;
		totalH += srcSurface->h;
		if (srcSurface->w > totalW) totalW = srcSurface->w;
		sourcesRect.push_back(srcRect);
		surfaces.push_back(srcSurface);
	}

	int power2 = surfaces[0]->format->BitsPerPixel;
	int depth = (power2 == 8) ? 32 : (power2 == 4) ? 16 : 8;

	SDL_Surface* allTogether = SDL_CreateRGBSurface(0, totalW, totalH, depth, 0, 0, 0, 0);

	SDL_Rect dstRect = { 0, 0, 0, 0 };
	for (auto i = 0; i < count; i++) {
		dstRect.w = sourcesRect[i].w;
		dstRect.h = sourcesRect[i].h;
		SDL_BlitSurface(surfaces[i], &sourcesRect[i], allTogether, &dstRect);
		dstRect.y += sourcesRect[i].h;

		SDL_FreeSurface(surfaces[i]);
		surfaces[i] = nullptr;
	}

	texture = SDL_CreateTextureFromSurface(mRenderer, allTogether);
	SDL_FreeSurface(allTogether);
}

void CBaseScreen::changeTextSize(int newTextSize)
{
	TTF_CloseFont(mFont);
	mFont = TTF_OpenFont(mFontName, mFontSize);
}

void CBaseScreen::onKeyDown(const SDL_Event& event)
{
    mInputEvents[event.key.keysym.sym] = true;
}

void CBaseScreen::onKeyUp(const SDL_Event& event)
{
    mInputEvents[event.key.keysym.sym] = false;
}
