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

#include <cstdio>

using namespace std;

Uint32 my_callbackfn(Uint32 interval, void* param)
{
	cout << "--- MY CALLBACK FN ---\n";
	if (param) {
		CBaseScreen* m = reinterpret_cast<CBaseScreen*>(param);
		if (m) {
			m->resetTimer();
		}
	}
	return 0u;
}

CBaseScreen::CBaseScreen(CMainDispatcher& dispatcher, TTF_Font* font)
: mDispatcher(dispatcher)
, mFont(font)
{
	cout << __PRETTY_FUNCTION__ << " [ctor]\n";
}

CBaseScreen::~CBaseScreen()
{
	cout << __PRETTY_FUNCTION__ << " [dtor]\n";
	if (mTimerID) {
		SDL_RemoveTimer(mTimerID);
	}
}

void CBaseScreen::init(SDL_Renderer* renderer)
{
	mAlive = true;
	mDispatcher.onComplete(this);
}

bool CBaseScreen::run(SDL_Window* window, SDL_Renderer* renderer, double)
{
	cout << __PRETTY_FUNCTION__ << "\n";
	if (mAlive) {
		processInputEvents(100);
		drawFrame(window, renderer);
	}
	return mAlive;
}

void CBaseScreen::onPrepare(int perc)
{
}

void CBaseScreen::cleanup(IPlayable* playable)
{
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
	mAlive = false;
}

void CBaseScreen::processInputEvents(int delayMs)
{

	SDL_Event event ;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mDispatcher.quit();
			break;
		case SDL_KEYDOWN:
			if (mTimerID == 0) {
				if (delayMs != 0) mTimerID = SDL_AddTimer(delayMs, &my_callbackfn, this);
				onKeyDown(event);
			}
			break;
		case SDL_KEYUP:
				onKeyUp(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
			break;
		}
	}
}

void CBaseScreen::drawFrame(SDL_Window* , SDL_Renderer* )
{
}

void CBaseScreen::sendOnComplete() {
	mDispatcher.onComplete(this);
}

SDL_Surface* CBaseScreen::drawMenuTextBlock(SDL_Renderer* renderer,
									const char** strings, const int count,
									int selection,
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
	printf("TOGETHER: %p\n", (void*)allTogether);
	SDL_Rect dstRect = {0, 0, 0, 0};
	for (auto i = 0; i < count; i++) {
		dstRect.w = sourcesRect[i].w;
		dstRect.h = sourcesRect[i].h;
		SDL_BlitSurface(surfaces[i], &sourcesRect[i], allTogether, &dstRect);
		dstRect.y += sourcesRect[i].h;

		SDL_FreeSurface(surfaces[i]);
		surfaces[i] = nullptr;
	}

	return allTogether;
}

void CBaseScreen::onKeyDown(const SDL_Event& event)
{
}

void CBaseScreen::onKeyUp(const SDL_Event& event)
{
}
