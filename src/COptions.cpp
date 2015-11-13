/*
 * COptions.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#include "COptions.hpp"

#include "CMainDispatcher.hpp"

#include <iostream>
#include <fstream>

#include <cstdio>

using namespace std;

const int OPTIONS_ELEMENTS = 3;
const char* OPTIONS_TEXT[OPTIONS_ELEMENTS] = { "ENABLE SOUND", "ENABLE MUSIC", "EXIT" };

const char* const OPTIONS_FILE_NAME = "options.dat";

COptions::COptions(CMainDispatcher& dispatcher, TTF_Font* font)
: CBaseScreen(dispatcher, font)
{
	cout << __PRETTY_FUNCTION__ << " [ctor]\n";
}

COptions::~COptions()
{
	SDL_DestroyTexture(mOptionsTexture);
	cout << __PRETTY_FUNCTION__ << " [dtor]\n";
}

void COptions::init(SDL_Renderer* renderer)
{
	cout << __PRETTY_FUNCTION__ << "\n";
	readOptionsFromFile();

	::CBaseScreen::init(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

bool COptions::run(SDL_Window* window, SDL_Renderer* renderer) {
	if (CBaseScreen::isAlive()) {
		CBaseScreen::processInputEvents(200);
	}
	if (CBaseScreen::isAlive()) {
		drawFrame(window, renderer);
	}
	return ::CBaseScreen::isAlive();
}

void COptions::onPrepare(int perc) {
	cout << __PRETTY_FUNCTION__ << " " << perc << "%\n";
}

bool COptions::isAlive() const {
	return CBaseScreen::isAlive();
}

void COptions::cleanup(IPlayable* playable) {
	cout << __PRETTY_FUNCTION__ << "\n";
}

void COptions::drawFrame(SDL_Window* window, SDL_Renderer* renderer) {
	SDL_Rect aDstRect = {100, 100, 0, 0};
	SDL_Rect aSrcRect = {0, 0, 0, 0};

	if (mUpdated) {

		if (mOptionsTexture) {
			SDL_DestroyTexture(mOptionsTexture);
			mOptionsTexture = nullptr;
		}

		SDL_Surface* tempSurface = CBaseScreen::drawMenuTextBlock(renderer,
				OPTIONS_TEXT, OPTIONS_ELEMENTS,	mSelection);
		printf("> TOGETHER: %p\n", (void*)tempSurface);

		if (tempSurface) {
			printf("w: %d, h: %d\n", tempSurface->w, tempSurface->h);
			aSrcRect.w = aDstRect.w = tempSurface->w;
			aSrcRect.h = aDstRect.h = tempSurface->h;

			mOptionsTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
			SDL_FreeSurface(tempSurface);
	
			SDL_RenderCopy(renderer, mOptionsTexture, &aSrcRect, &aDstRect);
		}
		mUpdated = false;
	}

	//
	//SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void COptions::readOptionsFromFile() {
	ifstream fs(OPTIONS_FILE_NAME);

	cout << __PRETTY_FUNCTION__ << " Open: " << std::boolalpha << fs.is_open() << "\n";
	if (!fs.is_open()) {
		cout << "No file.\n";
	} else {
		fs >> mSoundEnabled >> mMusicEnabled;
		fs.close();
		cout << std::boolalpha << "Sound : " << mSoundEnabled << " Music: " << mMusicEnabled << "\n";
	}
}

void COptions::updateOptionsFile() {
	ofstream fs(OPTIONS_FILE_NAME);

	cout << __PRETTY_FUNCTION__ << " Open: " << std::boolalpha << fs.is_open() << "\n";
	if (!fs.is_open()) {
		cout << "Creating new file.\n";
	}
	fs << mSoundEnabled << mMusicEnabled;
	fs.close();
}

void COptions::onKeyUp(const SDL_Event&)
{	
}

void COptions::onKeyDown(const SDL_Event& event)
{
	if (event.key.keysym.sym == SDLK_DOWN) {
		if (mSelection++ < OPTIONS_ELEMENTS - 1); else { mSelection = 0; }
		mUpdated = true;
	}
	else if (event.key.keysym.sym == SDLK_UP) {
		if (mSelection-- > 0); else { mSelection = OPTIONS_ELEMENTS - 1; }
		mUpdated = true;
	}
	else if (event.key.keysym.sym == SDLK_RETURN) {
		switch (mSelection) {
		case 0:
			break;
		case 1:
			break;
		default:
		case 2:
			updateOptionsFile();
			CBaseScreen::quit();
			break;
		}
	}
}
