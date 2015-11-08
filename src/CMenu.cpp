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

CMenu::CMenu(CMainDispatcher& dispatcher)
: mDispatcher(dispatcher)
, mWindow(nullptr)
, mRenderer(nullptr)
, mFont(nullptr)
, mEvents()
, mAlive(false)
, mSelection(0)
, mMenuTexture(nullptr)
, mInputTimerEnable(false)
, mTickCounter(0)
, mOptions(nullptr)
{
	cout << __FUNCTION__ << " [ctor]\n";
}

CMenu::~CMenu()
{
	cout << __FUNCTION__ << " [dtor]\n";
}

void CMenu::init(SDL_Window* window, SDL_Renderer* renderer)
{
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

bool CMenu::run()
{
	if (mAlive) {
		SDL_Event event ;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					mDispatcher.onDestroy(this);
					mAlive = false;
					break;
				case SDL_KEYDOWN:
					mInputTimerEnable = true;
					mTickCounter = 0;
					onKeyDown(event);
					break;
				case SDL_KEYUP:
					onKeyUp(event);
					break ;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
					break ;
			}
		}
		drawFrame();

		if (mTickCounter == 0) {
			if (mEvents[SDLK_DOWN]) {
				mSelection++; if (mSelection >= MENU_ELEMENTS) mSelection = 0;
			} else if (mEvents[SDLK_UP]) {
				mSelection--; if (mSelection < 0) mSelection = MENU_ELEMENTS - 1;
			} else if (mEvents[SDLK_RETURN]) {
				processInput();
			}
		}

		if (mInputTimerEnable) {
			if (mTickCounter++ > 100)  {
				mInputTimerEnable = false; mTickCounter = 0;
			}
		}
	}
	return mAlive;
}

void CMenu::onPrepare(int perc) {
	cout << __FUNCTION__ << " " << perc << "%\n";
}

bool CMenu::isAlive() const {
	return mAlive;
}

void CMenu::cleanup() {
	cout << __FUNCTION__ << "\n";
	if (mOptions) {
		delete mOptions;
	}
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

void CMenu::drawFrame() {
	SDL_Rect aDstRect = {100, 100, 0, 0};
	SDL_Rect aSrcRect = {0, 0, 0, 0};

	// Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	SDL_Color textColor = 	{0xff, 0xaa, 0x00, 0};
	SDL_Color selectColor = {0xff, 0xff, 0xff, 0};
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
	mMenuTexture = SDL_CreateTextureFromSurface(mRenderer, allTogether);

	SDL_FreeSurface(allTogether);

	SDL_RenderCopy(mRenderer, mMenuTexture, &aSrcRect, &aDstRect);

	SDL_RenderPresent(mRenderer);
}

void CMenu::processInput() {
	cout << __FUNCTION__ << " selection: " << mSelection << "\n";
	switch (mSelection) {
	case 0:
		break;
	case 1:
		mOptions = new COptions(mDispatcher);
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





