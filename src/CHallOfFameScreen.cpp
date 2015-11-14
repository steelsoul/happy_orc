/*
 * CHallOfFameScreen.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: alexander
 */

#include "CHallOfFameScreen.hpp"
#include "settings.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

using namespace std;

CHallOfFameScreen::CHallOfFameScreen(CMainDispatcher& dispatcher, TTF_Font* font)
: CBaseScreen(dispatcher, font)
, mFont(font)
, mBestTexture(nullptr)
, mBest(5, 0)
{
	cout << __PRETTY_FUNCTION__ << "[ctor]\n";
}

CHallOfFameScreen::~CHallOfFameScreen()
{
	SDL_DestroyTexture(mBestTexture);
	cout << __PRETTY_FUNCTION__ << "[dtor]\n";
}

void CHallOfFameScreen::init(SDL_Renderer* renderer)
{
	cout << __PRETTY_FUNCTION__ << "\n";

	// read best score
	fstream fs(HALLOFFAME_FILE_NAME, fstream::in | fstream::out);

	cout << __PRETTY_FUNCTION__ << " Open: " << std::boolalpha << fs.is_open() << "\n";
	if (!fs.is_open()) {
		cout << "Creating file.\n";
		fstream fsout(HALLOFFAME_FILE_NAME, fstream::out);

		for (auto a: mBest) {
			fsout << a;
		}
		fsout.close();
	} else {
		for (auto &a: mBest) {
			fs >> a;
		}
		cout << "Data read.\n";
	}

	fs.close();

	CBaseScreen::init(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

bool CHallOfFameScreen::run(SDL_Window* window, SDL_Renderer* renderer, double dt)
{
	if (CBaseScreen::isAlive()) drawFrame(window, renderer);

	CBaseScreen::processInputEvents(20);
	return CBaseScreen::isAlive();
}

void CHallOfFameScreen::onPrepare(int perc)
{
	cout << __PRETTY_FUNCTION__ << "\n";
}

void CHallOfFameScreen::cleanup(IPlayable* playable)
{
	cout << __PRETTY_FUNCTION__ << "\n";
}

void CHallOfFameScreen::onKeyDown(const SDL_Event& event)
{
	CBaseScreen::quit();
}

void CHallOfFameScreen::onKeyUp(const SDL_Event& event)
{

}

void CHallOfFameScreen::drawFrame(SDL_Window* window, SDL_Renderer* renderer)
{
	if (!mBestTexture) {
		vector<string> data;
		for (int i = 0; i < mBest.size(); i++)
		{
			string temp = to_string(i+1) + "         " + to_string(mBest[i]) + " PTS";
			data.push_back(temp);
		}
		data.push_back("               ");
		data.push_back("PRESS ANY KEY");

		SDL_Surface* textSurface = CBaseScreen::drawMenuTextBlock(renderer, data);
		mBestTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
	}
	SDL_Rect aSrcRect = {0, 0, 0, 0};
	SDL_Rect aDstRect = {100, 100, 0, 0};

	SDL_QueryTexture(mBestTexture, 0, 0, &aSrcRect.w, &aSrcRect.h);
	aDstRect.w = aSrcRect.w; aDstRect.h = aSrcRect.h;

	SDL_RenderCopy(renderer, mBestTexture, &aSrcRect, &aDstRect);
	SDL_RenderPresent(renderer);
}
