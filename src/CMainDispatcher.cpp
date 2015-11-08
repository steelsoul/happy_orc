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

CMainDispatcher::CMainDispatcher()
: mWindow(nullptr)
, mRenderer(nullptr)
, mPlayables()
, mIsRunning(initSDL())
, mPrepared(false)
{
	cout << __FUNCTION__ << "[ctor]\n";
}

CMainDispatcher::~CMainDispatcher() {
	cout << __FUNCTION__ << "[dtor]\n";
	shutdownSDL();
}

void CMainDispatcher::setNewPlayable(IPlayable* playable) {
	cout << __FUNCTION__ << "\n";

	mPlayables.push_back(playable);
	mPrepared = false;
	playable->init(mWindow, mRenderer);
}

void CMainDispatcher::onPrepare(int percentage) {
	cout << __FUNCTION__ << " " << percentage << "%\n";

	if (!mPlayables.empty()) {
		mPlayables.back()->onPrepare(percentage);
	}
}

void CMainDispatcher::onComplete() {
	cout << __FUNCTION__ << "\n";

	mPrepared = true;
	SDL_ShowWindow(mWindow);
}

void CMainDispatcher::onDestroy(IPlayable* which)
{
	cout << __FUNCTION__ << "\n"; cout.flush();
	for (auto p = mPlayables.begin(); p != mPlayables.end(); p++) {
		if (*p == which) {
		mPlayables.erase(p);
		cout << "0000\n"; cout.flush();
		if (!mPlayables.empty()) mPlayables.back()->cleanup();
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

	return result;
}

void CMainDispatcher::shutdownSDL() {
	cout << __FUNCTION__ << "\n";

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
	cout << __FUNCTION__ << "\n";
}

void CMainDispatcher::play() {
	cout << __FUNCTION__ << "\n"; cout.flush();

	//static int counter = 0;

	while (!mPlayables.empty()) {
//		cout << "1"; cout.flush();
		if (mPrepared && !mPlayables.empty()) {
//			cout << "0"; cout.flush();
			mIsRunning = mPlayables.back()->run();
		}
//		cout << "2"; cout.flush();
		//if (counter++ > 200) break;
		SDL_Delay(10);
	}
}

