/*
 * CMenu.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: alexander
 */


#include "CMainDispatcher.hpp"
#include <iostream>
#include "CGame.hpp"
#include "CMenu.hpp"

#include "settings.hpp"

using namespace std;

CMainDispatcher::CMainDispatcher(TTF_Font* font)
: mWindow(nullptr)
, mRenderer(nullptr)
, mFont(font)
, mPlayables()
, mQuit(!initSDL())
{
	cout << __PRETTY_FUNCTION__ << "[ctor]\n";
}

CMainDispatcher::~CMainDispatcher() {
	cout << __PRETTY_FUNCTION__ << "[dtor]\n";
	shutdownSDL();
}

void CMainDispatcher::setNewPlayable(IPlayable* playable) {
	cout << __PRETTY_FUNCTION__ << "\n";

	if (playable) {
		mNewPlayable = playable;
		mNewPlayable->init(mRenderer);
	}
}

void CMainDispatcher::onPrepare(int percentage) {
	cout << __PRETTY_FUNCTION__ << " " << percentage << "%\n";

	if (!mPlayables.empty()) {
		mPlayables.back()->onPrepare(percentage);
	}
}

void CMainDispatcher::onComplete(IPlayable* playable) {
	cout << __PRETTY_FUNCTION__ << "\n";

	if (playable == mNewPlayable) {
		mPlayables.push_back(mNewPlayable);
		mNewPlayable = nullptr;
	}
}

void CMainDispatcher::onDestroy(IPlayable* which)
{
	cout << __PRETTY_FUNCTION__ << "\n"; cout.flush();
	for (auto p = mPlayables.begin(); p != mPlayables.end(); p++) {
		if (*p == which) {
			mToRemovePlayable = which;
			mPlayables.erase(p);
//			if (!mPlayables.empty()) {
//				*(p-1)->init(mRenderer);
//			}
			break;
		}
	}
}

bool CMainDispatcher::initSDL()
{
	int flags = SDL_WINDOW_HIDDEN /*| SDL_WINDOW_FULLSCREEN*/ ;
	bool result = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		result = false;
	}
	/* Initialize the TTF library */
	if (result && TTF_Init() < 0) {
		result = false;
	}

	if (result &&
		SDL_CreateWindowAndRenderer(DISPLAY_WIDTH,
									DISPLAY_HEIGHT,
									flags, &mWindow,
									&mRenderer) < 0)
	{
		result = false;
	}

	if (!result) {
		cerr << "Couldn't initialize SDL: " << SDL_GetError() << "\n";
	}

	if (result)
	{
		SDL_ShowWindow(mWindow);
	}
	return result;
}

void CMainDispatcher::shutdownSDL() {
	cout << __PRETTY_FUNCTION__ << "\n";

    if (NULL != mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    if (NULL != mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    SDL_Quit();
}

void CMainDispatcher::switchPlayable() {
	cout << __PRETTY_FUNCTION__ << "\n";
}

void CMainDispatcher::play() {
	cout << __PRETTY_FUNCTION__ << "\n";

	while (!mQuit) {
		if (!mPlayables.empty()) {
			IPlayable* playscreen = mPlayables.back();
			if (playscreen) {
				if (mToRemovePlayable) {
					playscreen->cleanup(mToRemovePlayable);
					mToRemovePlayable = nullptr;
				}
				playscreen->run(mWindow, mRenderer);
			}
		} else {
			mQuit = true;
		};
		SDL_Delay(40u);
	}

}

void CMainDispatcher::quit() {
	cout << __PRETTY_FUNCTION__ << "\n";
	mQuit = true;
}

