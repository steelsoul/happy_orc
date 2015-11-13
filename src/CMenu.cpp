/*
 * CMainMenu.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: alexander
 */

#include "CMenu.hpp"
#include "CMainDispatcher.hpp"
#include "COptions.hpp"

#include <iostream>

using namespace std;

const int MENU_ELEMENTS = 4;
const char* MENU_TEXT[MENU_ELEMENTS] = { "START GAME", "OPTIONS", "HALL OF FAME", "EXIT" };

Uint32 my_callbackfn2(Uint32 interval, void* param)
{
	if (param) {
		CMenu* m = reinterpret_cast<CMenu*>(param);
		if (m) {
			m->onTimer();
		}
	}
	return interval;
}

CMenu::CMenu(CMainDispatcher& dispatcher, TTF_Font* font)
: mDispatcher(dispatcher)
, mFont(font)
, mEvents()
, mAlive(false)
, mSelection(0)
, mMenuTexture(nullptr)
, mTimerId(0)
, mOptions(nullptr)
, mGame(nullptr)
{
	cout << __PRETTY_FUNCTION__ << " [ctor]\n";
}

CMenu::~CMenu()
{
	cout << __PRETTY_FUNCTION__ << " [dtor]\n";
}

void CMenu::init(SDL_Renderer* renderer)
{
	mAlive = true;
	mDispatcher.onComplete(this);
}

bool CMenu::run(SDL_Window* window, SDL_Renderer* renderer)
{
	if (mAlive) {
		SDL_Event event ;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				mDispatcher.onDestroy(this);
				mAlive = false;
				return mAlive;
			case SDL_KEYDOWN:
				if (mTimerId == 0) {
					mTimerId = SDL_AddTimer(100, &my_callbackfn2, this);
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
		if (mChanged) {
			drawFrame(window, renderer);
			mChanged = false;
		}


		if (mEvents[SDLK_DOWN]) {
			mSelection++; if (mSelection >= MENU_ELEMENTS) mSelection = 0;
			mChanged = true;
			mEvents.clear();
		} else if (mEvents[SDLK_UP]) {
			mSelection--; if (mSelection < 0) mSelection = MENU_ELEMENTS - 1;
			mChanged = true;
			mEvents.clear();
		} else if (mEvents[SDLK_RETURN]) {
			processInput();
			mEvents.clear();
		}

	}
	return mAlive;
}

void CMenu::onPrepare(int perc) {
	cout << __PRETTY_FUNCTION__ << " " << perc << "%\n";
}

bool CMenu::isAlive() const {
	return mAlive;
}

void CMenu::cleanup(IPlayable* playable) {
	cout << __PRETTY_FUNCTION__ << "\n";
	if (mOptions == playable) {
		delete mOptions;
		mOptions = nullptr;
	} else if (mGame == playable) {
		delete mGame;
		mGame = nullptr;
	}
	mChanged = true;
}

void CMenu::quit() {
	mAlive = false;
}

void CMenu::onKeyDown(const SDL_Event& event)
{
	mEvents[event.key.keysym.sym] = 1;
}

void CMenu::onKeyUp(const SDL_Event& event)
{
	mEvents[event.key.keysym.sym] = 0;
}

void CMenu::drawFrame(SDL_Window* window, SDL_Renderer* renderer)
{
	if (!renderer) return;

	SDL_Rect aDstRect = {100, 100, 0, 0};
	SDL_Rect aSrcRect = {0, 0, 0, 0};

	SDL_Color textColor = 	{0xff, 0xaa, 0x00, 0u};
	SDL_Color selectColor = {0xff, 0xff, 0xff, 0u};
	SDL_Color color = textColor;

	SDL_Surface* surfaces[4];
	SDL_Rect sourcesRect[4];
	int totalW = 0; int totalH = 0;


	int depth = 0;
	for (auto i = 0; i < MENU_ELEMENTS; i++) {
		color = (i == mSelection) ? selectColor : textColor;
		surfaces[i] = TTF_RenderText_Solid(mFont, MENU_TEXT[i], color);
		sourcesRect[i].x = sourcesRect[i].y = 0;
		sourcesRect[i].w = surfaces[i]->w;
		sourcesRect[i].h = surfaces[i]->h;
		totalH += surfaces[i]->h;
		if (surfaces[i]->w > totalW) totalW = surfaces[i]->w;

	}

	int power2 = surfaces[0]->format->BitsPerPixel;
	depth = (power2 == 8) ? 32 : (power2 == 4) ? 16 : 8;

	SDL_Surface* allTogether = SDL_CreateRGBSurface(0, totalW, totalH, depth, 0, 0, 0, 0);

	SDL_Rect dstRect = {0, 0, 0, 0};
	for (auto i = 0; i<MENU_ELEMENTS; i++){
		dstRect.w = sourcesRect[i].w;
		dstRect.h = sourcesRect[i].h;
		SDL_BlitSurface(surfaces[i], &sourcesRect[i], allTogether, &dstRect);
		dstRect.y += sourcesRect[i].h;

		SDL_FreeSurface(surfaces[i]);
	}

	aSrcRect.w = aDstRect.w = totalW;
	aSrcRect.h = aDstRect.h = totalH;

	if (mMenuTexture) {
		SDL_DestroyTexture(mMenuTexture);
	}
	mMenuTexture = SDL_CreateTextureFromSurface(renderer, allTogether);

	SDL_FreeSurface(allTogether);

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, mMenuTexture, &aSrcRect, &aDstRect);

	SDL_RenderPresent(renderer);
}

void CMenu::processInput() {
	cout << __PRETTY_FUNCTION__ << " selection: " << mSelection << "\n";
	switch (mSelection) {
	case 0:
		mGame = new CGame(mDispatcher, mFont);
		mDispatcher.setNewPlayable(mGame);
		break;
	case 1:
		if (!mOptions) mOptions = new COptions(mDispatcher, mFont);
		mDispatcher.setNewPlayable(mOptions);
		break;
	case 2:
		break;
	case 3:
	default:
		mAlive = false;
		mDispatcher.onDestroy(this);
		break;
	}
}

void CMenu::onTimer()
{
	mTimerId = 0;
}





