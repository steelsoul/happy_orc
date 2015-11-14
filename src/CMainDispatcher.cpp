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

#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

CMainDispatcher::CMainDispatcher(SDL_Window* window,
		SDL_Renderer* renderer, TTF_Font* font)
: mWindow(window)
, mRenderer(renderer)
, mFont(font)
, mPlayables()
, mQuit(false)
{
	cout << __PRETTY_FUNCTION__ << "[ctor]\n";
}

CMainDispatcher::~CMainDispatcher() {
	cout << __PRETTY_FUNCTION__ << "[dtor]\n";
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

