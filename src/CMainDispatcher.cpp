/*
 * CMenu.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: alexander
 */


#include "CMainDispatcher.hpp"
#include <iostream>

#include "CGameScreen.hpp"
#include "CMenuScreen.hpp"
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

	Uint32 past = SDL_GetTicks();
    Uint32 now = past, pastFps = past ;
    double deltaTime = 0.0;
	while (!mQuit) {
        int timeElapsed = 0 ;
		if (!mPlayables.empty()) {
			IPlayable* playscreen = mPlayables.back();
	        timeElapsed = (now=SDL_GetTicks()) - past ;
	        if (timeElapsed >= UPDATE_INTERVAL) {
	        	past = now;
	        	deltaTime = timeElapsed / 40.0;
	        }
			if (playscreen) {
				if (mToRemovePlayable) {
					playscreen->cleanup(mToRemovePlayable);
					mToRemovePlayable = nullptr;

					SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
					SDL_RenderClear(mRenderer);

				}
				if (playscreen->isAlive()) {
					playscreen->run(mWindow, mRenderer, deltaTime);
				}
			}
		} else {
			mQuit = true;
		};
		SDL_Delay(10u);
	}

	for (auto a = mPlayables.rbegin(); a != mPlayables.rend(); a++) {
		IPlayable* temp = *a;
		temp->cleanup(nullptr);
	}
	mPlayables.clear();
}

void CMainDispatcher::quit() {
	cout << __PRETTY_FUNCTION__ << "\n";
	mQuit = true;
}

