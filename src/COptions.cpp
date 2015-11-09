/*
 * COptions.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#include "COptions.hpp"

#include "CMainDispatcher.hpp"

#include <iostream>

using namespace std;

COptions::COptions(CMainDispatcher& dispatcher)
: mDispatcher(dispatcher)
, mWindow(nullptr)
, mRenderer(nullptr)
, mFont(nullptr)
, mAlive(false)
{
	cout << __FUNCTION__ << " [ctor]\n";
}

COptions::~COptions()
{
	cout << __FUNCTION__ << " [dtor]\n";
	mAlive = false;
}

void COptions::init(SDL_Window* window, SDL_Renderer* renderer)
{
	cout << __FUNCTION__ << "\n";

	mWindow = window;
	mRenderer = renderer;

	bool ttfInitResult = true;

	/* Initialize the TTF library */
	ttfInitResult = (TTF_Init() == 0);

	if (ttfInitResult) {
		mFont = TTF_OpenFont("Effinground.ttf", 30);
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

	cerr << "TTF library initialized: " << boolalpha << ttfInitResult << "\n";

	mAlive = true;
	mDispatcher.onComplete();
}

bool COptions::run() {
	if (mAlive) {
		SDL_Event event ;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					mAlive = false;
					mDispatcher.onDestroy(this);
					return mAlive;
				case SDL_KEYDOWN:
					mAlive = false;
					mDispatcher.onDestroy(this);
					break;
				case SDL_KEYUP:
					break ;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
					break ;
			}
		}
		drawFrame();
	}

	return mAlive;
}

void COptions::onPrepare(int perc) {
	cout << __FUNCTION__ << " " << perc << "%\n";
}

bool COptions::isAlive() const {
	return mAlive;
}

void COptions::cleanup() {
	cout << __FUNCTION__ << "\n";
}

void COptions::drawFrame() {
	// Clear screen
	if (mAlive) {
		SDL_SetRenderDrawColor(mRenderer, 0, 0xFF, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(mRenderer);

		SDL_RenderPresent(mRenderer);
	}
}

