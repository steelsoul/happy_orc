/*
 * CMainMenu.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: alexander
 */

#include "CMenuScreen.hpp"
#include "CMainDispatcher.hpp"
#include "COptionsScreen.hpp"

#include <iostream>

using namespace std;

const int MENU_ELEMENTS = 4;
const char* MENU_TEXT[MENU_ELEMENTS] = { "START GAME", "OPTIONS", "HALL OF FAME", "EXIT" };

CMenuScreen::CMenuScreen(CMainDispatcher& dispatcher, TTF_Font* font)
: CBaseScreen(dispatcher, font)
, mDispatcher(dispatcher)
, mFont(font)
, mSelection(0)
, mMenuTexture(nullptr)
, mOptions(nullptr)
, mGame(nullptr)
{
	cout << __PRETTY_FUNCTION__ << " [ctor]\n";
}

CMenuScreen::~CMenuScreen()
{
	cout << __PRETTY_FUNCTION__ << " [dtor]\n";
}

void CMenuScreen::init(SDL_Renderer* renderer)
{
	CBaseScreen::init(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

bool CMenuScreen::run(SDL_Window* window, SDL_Renderer* renderer, double)
{
	if (mChanged) {
		if (isAlive()) drawFrame(window, renderer);
		mChanged = false;
	}

	CBaseScreen::processInputEvents(20);
}

void CMenuScreen::onPrepare(int perc) {
	cout << __PRETTY_FUNCTION__ << " " << perc << "%\n";
}

void CMenuScreen::cleanup(IPlayable* playable) {
	cout << __PRETTY_FUNCTION__ << "\n";
	if (mOptions == playable || playable == nullptr) {
		delete mOptions;
		mOptions = nullptr;
	} else if (mGame == playable || playable == nullptr) {
		delete mGame;
		mGame = nullptr;
	}
	mChanged = true;

}

void CMenuScreen::onKeyDown(const SDL_Event& event)
{
	SDL_Keycode sym = event.key.keysym.sym;

	if (sym == SDLK_DOWN) {
		if (mSelection++ < MENU_ELEMENTS - 1); else { mSelection = 0; }
		mChanged = true;
	}
	else if (sym == SDLK_UP) {
		if (mSelection-- > 0); else { mSelection = MENU_ELEMENTS - 1; }
		mChanged = true;
	}
	else if (sym == SDLK_RETURN) {
		processInput();
	}
}

void CMenuScreen::onKeyUp(const SDL_Event& event)
{

}

void CMenuScreen::drawFrame(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_Rect aDstRect = {100, 100, 0, 0};
	SDL_Rect aSrcRect = {0, 0, 0, 0};

	SDL_Surface* tempSurface = CBaseScreen::drawMenuTextBlock(renderer, MENU_TEXT,
			MENU_ELEMENTS, mSelection);

	aSrcRect.w = aDstRect.w = tempSurface->w;
	aSrcRect.h = aDstRect.h = tempSurface->h;

	if (mMenuTexture) {
		SDL_DestroyTexture(mMenuTexture);
	}
	mMenuTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	SDL_RenderCopy(renderer, mMenuTexture, &aSrcRect, &aDstRect);
	SDL_RenderPresent(renderer);
}

void CMenuScreen::processInput() {
	cout << __PRETTY_FUNCTION__ << " selection: " << mSelection << "\n";
	switch (mSelection) {
	case 0:
		mGame = new CGameScreen(mDispatcher, mFont);
		mDispatcher.setNewPlayable(mGame);
		break;
	case 1:
		if (!mOptions) mOptions = new COptionsScreen(mDispatcher, mFont);
		mDispatcher.setNewPlayable(mOptions);
		break;
	case 2:
		break;
	case 3:
	default:
		CBaseScreen::quit();
		break;
	}
}






