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

using namespace std;

const int OPTIONS_ELEMENTS = 3;
const char* OPTIONS_TEXT[OPTIONS_ELEMENTS] = { "ENABLE SOUND", "ENABLE MUSIC", "EXIT" };

const char* const OPTIONS_FILE_NAME = "options.dat";

COptions::COptions(CMainDispatcher& dispatcher)
: CBaseScreen(dispatcher)
{
	cout << __FUNCTION__ << " [ctor]\n";
}

COptions::~COptions()
{
	cout << __FUNCTION__ << " [dtor]\n";
	updateOptionsFile();
}

void COptions::init(SDL_Window* window, SDL_Renderer* renderer)
{
	cout << __FUNCTION__ << "\n";
	readOptionsFromFile();

	::CBaseScreen::init(window, renderer);
}

bool COptions::run() {
	if (CBaseScreen::isAlive()) {
		CBaseScreen::processInputEvents(200);
		drawFrame();
	}
	return ::CBaseScreen::isAlive();
}

void COptions::onPrepare(int perc) {
	cout << __FUNCTION__ << " " << perc << "%\n";
}

bool COptions::isAlive() const {
	return CBaseScreen::isAlive();
}

void COptions::cleanup() {
	cout << __FUNCTION__ << "\n";
}

void COptions::drawFrame() {
	SDL_Rect aDstRect = {100, 100, 0, 0};
	SDL_Rect aSrcRect = {0, 0, 0, 0};

	// Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	SDL_Color textColor = 	{0xff, 0xaa, 0x00, 0};
	SDL_Color selectColor = {0xff, 0xff, 0xff, 0};
	SDL_Color color = textColor;

	SDL_Surface* surfaces[OPTIONS_ELEMENTS];
	SDL_Rect sourcesRect[OPTIONS_ELEMENTS];
	int totalW = 0; int totalH = 0;

	int depth = 0;
	for (auto i = 0; i < OPTIONS_ELEMENTS; i++) {
		color = (i == mSelection) ? selectColor : textColor;
		surfaces[i] = TTF_RenderText_Solid(mFont, OPTIONS_TEXT[i], color);
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
	for (auto i = 0; i < OPTIONS_ELEMENTS; i++){
		dstRect.w = sourcesRect[i].w;
		dstRect.h = sourcesRect[i].h;
		SDL_BlitSurface(surfaces[i], &sourcesRect[i], allTogether, &dstRect);
		dstRect.y += sourcesRect[i].h;

		SDL_FreeSurface(surfaces[i]);
	}

	aSrcRect.w = aDstRect.w = totalW;
	aSrcRect.h = aDstRect.h = totalH;

	if (mOptionsTexture) {
		SDL_DestroyTexture(mMenuTexture);
	}
	mOptionsTexture = SDL_CreateTextureFromSurface(mRenderer, allTogether);

	SDL_FreeSurface(allTogether);

	SDL_RenderCopy(mRenderer, mOptionsTexture, &aSrcRect, &aDstRect);

	SDL_RenderPresent(mRenderer);
}

void COptions::readOptionsFromFile() {
	ifstream fs(OPTIONS_FILE_NAME);

	cout << __FUNCTION__ << " Open: " << std::boolalpha << fs.is_open() << "\n";
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

	cout << __FUNCTION__ << " Open: " << std::boolalpha << fs.is_open() << "\n";
	if (!fs.is_open()) {
		cout << "Creating new file.\n";
	}
	fs << mSoundEnabled << mMusicEnabled;
	fs.close();
}

