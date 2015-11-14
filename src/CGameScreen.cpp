/*
 * CCGame.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "CHallOfFameScreen.hpp"
#include "CGameScreen.hpp"
#include "settings.hpp"

#include "overlaptester/COverlapTester.hpp"

#include "CMainDispatcher.hpp"

using namespace std;
using namespace happyorc;

#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

Uint32 my_timer_fn(Uint32 interval, void *param)
{
//	cout << "==My TIMER fn==\n"; cout.flush();
	if (param) {
		CGameScreen* game = reinterpret_cast<CGameScreen*>(param);
		if (game) game->startTimer();
	}
	return 0u;
}

CGameScreen::CGameScreen(CMainDispatcher& dispatcher, TTF_Font* font)
: CBaseScreen(dispatcher, font)
, mDispatcher(dispatcher)
, mPauseState(2)
, mBgTexture(nullptr)
, mOrcTexure(nullptr)
, mHamTexture(nullptr)
, mScoreTexture(nullptr)
, mScorepoints(0)
, mUpdateScores(true)
, mPausedTexture(nullptr)
, mFont(font)
, mAhero(0, 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, mAham(rand() % (DISPLAY_WIDTH - HAM_WIDTH), -HAM_WIDTH, HAM_WIDTH, HAM_HEIGHT, HAM_SPEED, DISPLAY_HEIGHT)
, mAmaster(rand() % (DISPLAY_WIDTH - ORC_WIDTH), 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, mMasterdata()
, mTimerID2(0)
, mInput()
{
	cout << __FUNCTION__ << "[ctor]\n";
	srand(static_cast<Uint32>(time(NULL)));
	mAhero.setX(mAmaster.getX());
}

CGameScreen::~CGameScreen() {
	cout << __FUNCTION__ << "[dtor]\n";
	this->stop();
}

void CGameScreen::init(SDL_Renderer* renderer)
{
	cerr << __PRETTY_FUNCTION__ << "\n";
	mBgTexture = IMG_LoadTexture(renderer, "forest_480x320.jpg");
	if (!mBgTexture) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	loadsprite(renderer, "Orcs.bmp", mOrcTexure);
	if (!mOrcTexure) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	loadsprite(renderer, "ham.bmp", mHamTexture);
	if (!mHamTexture) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	mAhero.setY(static_cast<int>(DISPLAY_HEIGHT*(1 - 0.2)));
	startTimer();

	CBaseScreen::init(renderer);
}

bool CGameScreen::run(SDL_Window* window, SDL_Renderer* renderer, double dt) {
	update(dt);

	if (isAlive()) drawFrame(window, renderer);

	return CBaseScreen::isAlive();
}

void CGameScreen::onPrepare(int perc) {
	cout << __FUNCTION__ << " " << perc << "%\n";
}

void CGameScreen::cleanup(IPlayable* playable) {
	cout << __FUNCTION__ << "\n";
}


void CGameScreen::startTimer() {
	mTimerID2 = SDL_AddTimer(500 + rand() % 800, my_timer_fn, this);
	//fprintf(stderr, "Start timer with ID %d\n", static_cast<int>(mTimerID2));

	/* update direction of master orc */
	mMasterdata.direction = rand() % 3;
	if (!mMasterdata.dropham) {
		if (rand() % 2 == 0) {
			mMasterdata.dropham = true;
	    	mAham.setX(mAmaster.getX() + (ORC_SPRITE_WIDTH - HAM_SPRITE_WIDTH)/2);
	    	mAham.setY(mAmaster.getY() + (ORC_SPRITE_HEIGHT - HAM_SPRITE_HEIGHT)/2);
		}
	}
//	fprintf(stdout, "master direction : %d\n", masterdata.direction);
}

void CGameScreen::drawFrame(SDL_Window* window, SDL_Renderer* renderer) {
	SDL_Rect aDstRect;
	SDL_Rect aSrcRect;

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (NULL != mBgTexture) {
		SDL_RenderCopy(renderer, mBgTexture, NULL, NULL);
	}

	//// Render hero
	aDstRect.x = mAhero.getX();
	aDstRect.y = mAhero.getY();
	aDstRect.w = ORC_WIDTH;
	aDstRect.h = ORC_HEIGHT;

    CRectangle spriteSrc = mAhero.getSrcRect();
    aSrcRect.x = static_cast<int>(spriteSrc.mLowerLeft.mx);
	aSrcRect.y = static_cast<int>(spriteSrc.mLowerLeft.my);
	aSrcRect.w = static_cast<int>(spriteSrc.mwidth);
	aSrcRect.h = static_cast<int>(spriteSrc.mheight);

	SDL_RenderCopy(renderer, mOrcTexure, &aSrcRect, &aDstRect);

	/// Render master
	aDstRect.x = mAmaster.getX();
	aDstRect.y = mAmaster.getY();
	aDstRect.w = ORC_WIDTH;
	aDstRect.h = ORC_HEIGHT;

    spriteSrc = mAmaster.getSrcRect();
    aSrcRect.x = static_cast<int>(spriteSrc.mLowerLeft.mx);
	aSrcRect.y = static_cast<int>(spriteSrc.mLowerLeft.my) + ORC_SPRITE_HEIGHT;
	aSrcRect.w = static_cast<int>(spriteSrc.mwidth);
	aSrcRect.h = static_cast<int>(spriteSrc.mheight);

	SDL_RenderCopy(renderer, mOrcTexure, &aSrcRect, &aDstRect);

	/// Render ham
	if (mMasterdata.dropham)
	{
		aDstRect.x = mAham.getX();
		aDstRect.y = mAham.getY();
		aDstRect.w = HAM_WIDTH;
		aDstRect.h = HAM_HEIGHT;

		spriteSrc = mAham.getSrcRect();
		aSrcRect.x = static_cast<int>(spriteSrc.mLowerLeft.mx);
		aSrcRect.y = static_cast<int>(spriteSrc.mLowerLeft.my);
		aSrcRect.w = static_cast<int>(spriteSrc.mwidth);
		aSrcRect.h = static_cast<int>(spriteSrc.mheight);

		SDL_RenderCopy(renderer, mHamTexture, &aSrcRect, &aDstRect);
	}
	drawScore(renderer);

	if (mPauseState == 1 || mPauseState == 0) {
		if (!mPausedTexture) {
			SDL_Color textColor = { 0xFF, 0xFF, 0x00, 0 };
			SDL_Surface * tempsurf = TTF_RenderText_Solid(mFont, "P A U S E D", textColor);

			mPausedTexture = SDL_CreateTextureFromSurface(renderer, tempsurf);
			SDL_FreeSurface(tempsurf);

			aSrcRect.x = 0;
			aSrcRect.y = 0;
			aSrcRect.w = tempsurf->w;
			aSrcRect.h = tempsurf->h;
		}

		aSrcRect = {0, 0, 0, 0};
		SDL_QueryTexture(mPausedTexture, 0, 0, &aSrcRect.w, &aSrcRect.h);

		aDstRect.x = DISPLAY_WIDTH/2 - aSrcRect.w/2;
		aDstRect.y = DISPLAY_HEIGHT/2 - aSrcRect.h/2;
		aDstRect.w = aSrcRect.w;
		aDstRect.h = aSrcRect.h;
		SDL_RenderCopy(renderer, mPausedTexture, &aSrcRect, &aDstRect);
	}

	SDL_RenderPresent(renderer);
}

void CGameScreen::drawScore(SDL_Renderer* renderer) {
	if (!mScoreTexture || mUpdateScores) {

		SDL_Color textColor = { 0xFF, 0x00, 0x00, 0 };
		std::stringstream ss;
		ss << mScorepoints << " PTS";
		SDL_Surface * tempsurf = TTF_RenderText_Solid(mFont, ss.str().c_str(), textColor);

		if (mScoreTexture) {
			SDL_DestroyTexture(mScoreTexture);
			mScoreTexture = NULL;
		}

		mScoreTexture = SDL_CreateTextureFromSurface(renderer, tempsurf);
		SDL_FreeSurface(tempsurf);
		mUpdateScores = false;
	}
	SDL_Rect textsource = {0, 0, 0, 0};
	SDL_QueryTexture(mScoreTexture, 0, 0, &textsource.w, &textsource.h);

	SDL_Rect dst;
	dst.x = 10; dst.y = static_cast<int>(DISPLAY_WIDTH * 0.2); dst.w = textsource.w; dst.h = textsource.h;
	SDL_RenderCopy(renderer, mScoreTexture, &textsource, &dst);
}


void CGameScreen::stop() {
    if (NULL != mBgTexture) {
    	SDL_DestroyTexture(mBgTexture);
    }
	if (NULL != mOrcTexure) {
		SDL_DestroyTexture(mOrcTexure);
	}
	if (NULL != mHamTexture) {
		SDL_DestroyTexture(mHamTexture);
	}
	if (NULL != mScoreTexture) {
		SDL_DestroyTexture(mScoreTexture);
	}

	if (mTimerID2 != 0) SDL_RemoveTimer(mTimerID2);
}

void CGameScreen::fillRect(SDL_Renderer* renderer, SDL_Rect* rc, int r, int g, int b ) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, rc);
}

void CGameScreen::fpsChanged( int fps ) {
}

void CGameScreen::onQuit() {
	fprintf(stdout, "Good bye!\n");
}

void CGameScreen::update(double deltaTime) {
	CBaseScreen::processInputEvents(0);

    if (mInput[SDLK_ESCAPE]) {
    	CBaseScreen::quit();
    }

    if (mPauseState == 1 || mPauseState == 0) return;

	if ( mInput[SDLK_LEFT] ) {
		mAhero.moveLeft(deltaTime);
    } else if ( mInput[SDLK_RIGHT] ) {
		mAhero.moveRight(deltaTime);
    }

    if (mMasterdata.direction == 0) {
    	mAmaster.moveLeft(deltaTime);
    } else if (mMasterdata.direction == 1) {
    	mAmaster.moveRight(deltaTime);
    }

    if (mMasterdata.dropham) {
    	mAham.moveDown(deltaTime);
    }

    if (mAham.getY() + HAM_WIDTH >= DISPLAY_HEIGHT*0.9)
    {
    	mMasterdata.dropham = false;
    	onLoose();
    }

    checkCollisions();
}

void CGameScreen::onKeyDown( const SDL_Event& evt ) {
    mInput[ evt.key.keysym.sym ] = true;

    if (evt.key.keysym.sym == SDLK_p)
    {
    	if (mPauseState == 0 ) {
    		mPauseState = 3;
    	} else if (mPauseState == 2) {
    		mPauseState = 1;
    	}
    }
}

void CGameScreen::onKeyUp( const SDL_Event& evt ) {
    mInput[ evt.key.keysym.sym ] = false;
    if (evt.key.keysym.sym == SDLK_p) {
    	if (mPauseState == 3) {
    		mPauseState = 2;
    	} else if (mPauseState == 1) {
    		mPauseState = 0;
    	}
    }
}

void CGameScreen::setKeyColor(SDL_Surface* surface)
{
	if (surface->format->palette) {
		SDL_SetColorKey(surface, 1, *(Uint8 *)surface->pixels);
	} else {
		switch (surface->format->BitsPerPixel) {
		case 15:
			SDL_SetColorKey(surface, 1, (*(Uint16 *)surface->pixels) & 0x00007FFF);
			break;
		case 16:
			SDL_SetColorKey(surface, 1, *(Uint16 *)surface->pixels);
			break;
		case 24:
			SDL_SetColorKey(surface, 1, (*(Uint32 *)surface->pixels) & 0x00FFFFFF);
			break;
		case 32:
			SDL_SetColorKey(surface, 1, *(Uint32 *)surface->pixels);
			break;
		}
	}
}

void CGameScreen::loadsprite(SDL_Renderer* renderer, const char* path, SDL_Texture*& texture)
{
	SDL_Surface* temp = SDL_LoadBMP(path);
	if (temp)
	{
		setKeyColor(temp);
		texture = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);
	} else {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}
}

void CGameScreen::checkCollisions() {
	using framework::math::COverlapTester;
	if (COverlapTester::overlapRectangles(mAhero.getBound(), mAham.getBound()))
	{
//		fprintf(stderr, "Overlaps\n");
		mScorepoints += 1;
		mUpdateScores = true;

		// reset aham positions
		mAham.setY(-HAM_WIDTH);
		mMasterdata.dropham = false;

		if (mScorepoints % 20 == 0 && mAham.getSpeed() < 10) {
			mAham.setSpeed(mAham.getSpeed() + static_cast<uint32_t>(mScorepoints / 20));
		}
	}
}

void CGameScreen::onLoose() {
	fprintf(stdout, "You loose with %d PTS. Have a nice day!\n", mScorepoints);
	CHallOfFameScreen::writeScorePoints(mScorepoints);
	CBaseScreen::quit();
}

