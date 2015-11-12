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
	
	if (mOptionsTexture) {
		SDL_DestroyTexture(mOptionsTexture);
		mOptionsTexture = nullptr;
	}
	
	CBaseScreen::drawMenuTextBlock(OPTIONS_TEXT, OPTIONS_ELEMENTS, mOptionsTexture, mSelection);
	
	if (mOptionsTexture) {
		int w; 
		int h;
		SDL_QueryTexture(mOptionsTexture, 0, 0, &w, &h);
		
		aSrcRect.w = aDstRect.w = w;
		aSrcRect.h = aDstRect.h = h;

		SDL_RenderCopy(mRenderer, mOptionsTexture, &aSrcRect, &aDstRect);
	}

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

void COptions::onKeyUp(const SDL_Event&)
{	
}

void COptions::onKeyDown(const SDL_Event& event)
{
	if (event.key.keysym.sym == SDLK_DOWN) {
		if (mSelection++ < OPTIONS_ELEMENTS - 1); else { mSelection = 0; }
	}
	else if (event.key.keysym.sym == SDLK_UP) {
		if (mSelection-- > 0); else { mSelection = OPTIONS_ELEMENTS - 1; }
	}
	else if (event.key.keysym.sym == SDLK_RETURN) {
		switch (mSelection) {
		case 0:
			break;
		case 1:
			break;
		default:
		case 2:
			CBaseScreen::quit();
			break;
		}
	}
}
