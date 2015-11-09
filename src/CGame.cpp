/*
 * CCGame.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include "CGame.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "settings.hpp"

#include "overlaptester/COverlapTester.hpp"

#include "CMainDispatcher.hpp"

using namespace std;
using namespace happyorc;

Uint32 my_timer_fn(Uint32 interval, void *param)
{
	//fprintf(stderr, "== on Time () ==\n");
	if (param) {
		CGame* game = reinterpret_cast<CGame*>(param);
		game->startTimer();
	}
	return 0u;
}

CGame::CGame(CMainDispatcher& dispatcher)
: mDispatcher(dispatcher)
, mWindow(nullptr)
, mRenderer(nullptr)
, keys()
, frameSkip(0u)
, mpaused(false)
, background(nullptr)
, orcs(nullptr)
, ham(nullptr)
, mFont(nullptr)
, mscore(nullptr)
, mscorepoints(0)
, paused(nullptr)
, ahero(0, 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, aham(rand() % (DISPLAY_WIDTH - HAM_WIDTH), -HAM_WIDTH, HAM_WIDTH, HAM_HEIGHT, HAM_SPEED, DISPLAY_HEIGHT)
, amaster(rand() % (DISPLAY_WIDTH - ORC_WIDTH), 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, masterdata()
, mAlive(false)
, mTimerID(0)
{
	cout << __FUNCTION__ << "[ctor]\n";
	srand(time(NULL));
	aham.setX(rand() % (DISPLAY_WIDTH - HAM_WIDTH));
}

CGame::~CGame() {
	cout << __FUNCTION__ << "[dtor]\n";
	this->stop();
}

void CGame::init(SDL_Window* window, SDL_Renderer* renderer)
{
	mWindow = window;
	mRenderer = renderer;

	bool ttfInitResult = true;

	/* Initialize the TTF library */
	ttfInitResult = (TTF_Init() == 0);

	if (ttfInitResult) {
		mFont = TTF_OpenFont("Effinground.ttf", 30);
		if (!mFont) {
			ttfInitResult = false;
		}
	}

	if (ttfInitResult) {
		TTF_SetFontStyle(mFont, TTF_STYLE_NORMAL | TTF_STYLE_BOLD);
		TTF_SetFontOutline(mFont, 0);
		TTF_SetFontKerning(mFont, 0);
		TTF_SetFontHinting(mFont, TTF_HINTING_NORMAL);
	}

	cerr << "TTF library initialized: " << boolalpha << ttfInitResult << "\n";

	background = IMG_LoadTexture(renderer, "forest_480x320.jpg");
	if (!background) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	loadsprite("Orcs.bmp", orcs);
	if (!orcs) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	loadsprite("ham.bmp", ham);
	if (!ham) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	ahero.setY(static_cast<int>(DISPLAY_HEIGHT*(1 - 0.2)));
	startTimer();

	mAlive = true;
	mDispatcher.onComplete();
}

bool CGame::run() {	
	SDL_Event event;

	if (mAlive) {
		if (SDL_PollEvent(&event)) 
		{
			switch (event.type) {
			case SDL_QUIT:
				mAlive = false;
				mDispatcher.onDestroy(this);
				//mDispatcher.quit();
				//onQuit(); 
				break;
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					mDispatcher.onDestroy(this);
					mAlive = false;
				}
				if (event.key.keysym.sym == SDLK_p) {
					mpaused = !mpaused;
				}

				onKeyDown(&event);
			}
				break;
			case SDL_KEYUP:   
				onKeyUp(&event);   
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				break;
			}
		}

		if (mAlive) {

			if (!mpaused) {
				update(0.1); // TODO: fix this
			}
			draw(); 
		}
	}

	return mAlive;
}

void CGame::onPrepare(int perc) {
	cout << __FUNCTION__ << " " << perc << "%\n";
}

bool CGame::isAlive() const {
	return mAlive;
}

void CGame::cleanup() {
	cout << __FUNCTION__ << "\n";
}


void CGame::startTimer() {
	mTimerID = SDL_AddTimer(500 + rand() % 800, my_timer_fn, this);
//	fprintf(stderr, "Start timer with ID %d\n", static_cast<int>(tid));

	/* update direction of master orc */
	masterdata.direction = rand() % 3;
	if (!masterdata.dropham) {
		if (rand() % 2 == 0) {
			masterdata.dropham = true;
	    	aham.setX(amaster.getX() + (ORC_SPRITE_WIDTH - HAM_SPRITE_WIDTH)/2);
	    	aham.setY(amaster.getY() + (ORC_SPRITE_HEIGHT - HAM_SPRITE_HEIGHT)/2);
		}
	}
//	fprintf(stdout, "master direction : %d\n", masterdata.direction);
}

void CGame::draw() {
	SDL_Rect aDstRect;
	SDL_Rect aSrcRect;

	// Clear screen
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	if (NULL != background) {
		SDL_RenderCopy(mRenderer, background, NULL, NULL);
	}

	//// Render hero
	aDstRect.x = ahero.getX();
	aDstRect.y = ahero.getY();
	aDstRect.w = ORC_WIDTH;
	aDstRect.h = ORC_HEIGHT;

    CRectangle spriteSrc = ahero.getSrcRect();
    aSrcRect.x = static_cast<int>(spriteSrc.mLowerLeft.mx);
	aSrcRect.y = static_cast<int>(spriteSrc.mLowerLeft.my);
	aSrcRect.w = static_cast<int>(spriteSrc.mwidth);
	aSrcRect.h = static_cast<int>(spriteSrc.mheight);

	SDL_RenderCopy(mRenderer, orcs, &aSrcRect, &aDstRect);

	/// Render master
	aDstRect.x = amaster.getX();
	aDstRect.y = amaster.getY();
	aDstRect.w = ORC_WIDTH;
	aDstRect.h = ORC_HEIGHT;

    spriteSrc = amaster.getSrcRect();
    aSrcRect.x = static_cast<int>(spriteSrc.mLowerLeft.mx);
	aSrcRect.y = static_cast<int>(spriteSrc.mLowerLeft.my) + ORC_SPRITE_HEIGHT;
	aSrcRect.w = static_cast<int>(spriteSrc.mwidth);
	aSrcRect.h = static_cast<int>(spriteSrc.mheight);

	SDL_RenderCopy(mRenderer, orcs, &aSrcRect, &aDstRect);

	/// Render ham
	if (masterdata.dropham)
	{
		aDstRect.x = aham.getX();
		aDstRect.y = aham.getY();
		aDstRect.w = HAM_WIDTH;
		aDstRect.h = HAM_HEIGHT;

		spriteSrc = aham.getSrcRect();
		aSrcRect.x = static_cast<int>(spriteSrc.mLowerLeft.mx);
		aSrcRect.y = static_cast<int>(spriteSrc.mLowerLeft.my);
		aSrcRect.w = static_cast<int>(spriteSrc.mwidth);
		aSrcRect.h = static_cast<int>(spriteSrc.mheight);

		SDL_RenderCopy(mRenderer, ham, &aSrcRect, &aDstRect);
	}
	drawScore();

	if (mpaused) {
		if (paused) {
			SDL_DestroyTexture(paused);
		}
		SDL_Color textColor = { 0xFF, 0xFF, 0x00, 0 };
		SDL_Surface * tempsurf = TTF_RenderText_Solid(mFont, "P A U S E D", textColor);
		
		paused = SDL_CreateTextureFromSurface(mRenderer, tempsurf);
		
		aSrcRect.x = 0;
		aSrcRect.y = 0;
		aSrcRect.w = tempsurf->w;
		aSrcRect.h = tempsurf->h;

		aDstRect.x = DISPLAY_WIDTH/2 - aSrcRect.w/2;
		aDstRect.y = DISPLAY_HEIGHT/2 - aSrcRect.h/2;
		aDstRect.w = aSrcRect.w;
		aDstRect.h = aSrcRect.h;

		aSrcRect.x = 0;
		aSrcRect.y = 0;
		aSrcRect.w = tempsurf->w;
		aSrcRect.h = tempsurf->h;

		SDL_FreeSurface(tempsurf);
		SDL_RenderCopy(mRenderer, paused, &aSrcRect, &aDstRect);
	}

	SDL_RenderPresent(mRenderer);
}

void CGame::drawScore() {
	SDL_Color textColor = { 0xFF, 0x00, 0x00, 0 };
	std::stringstream ss;
	ss << mscorepoints << " PTS";
	SDL_Surface * tempsurf = TTF_RenderText_Solid(mFont, ss.str().c_str(), textColor);

	if (mscore) {
		SDL_DestroyTexture(mscore);
		mscore = NULL;
	}

	mscore = SDL_CreateTextureFromSurface(mRenderer, tempsurf);

	SDL_Rect textsource;

	textsource.x = 0;
	textsource.y = 0;
	textsource.w = tempsurf->w; 
	textsource.h = tempsurf->h;

	SDL_FreeSurface(tempsurf);

	SDL_Rect dst;
	dst.x = 10; dst.y = DISPLAY_WIDTH * 0.2; dst.w = textsource.w; dst.h = textsource.h;
	SDL_RenderCopy(mRenderer, mscore, &textsource, &dst);
}


void CGame::stop() {

 //   if (NULL != background) {
 //   	SDL_DestroyTexture(background);
 //   }
	//if (NULL != orcs) {
	//	SDL_DestroyTexture(orcs);
	//}
	//if (NULL != ham) {
	//	SDL_DestroyTexture(ham);
	//}
	//if (NULL != mscore) {
	//	SDL_DestroyTexture(mscore);
	//}

	//TTF_CloseFont(mFont);
	//TTF_Quit();
	if (mTimerID) SDL_RemoveTimer(mTimerID);
}

void CGame::fillRect(SDL_Rect* rc, int r, int g, int b ) {
    SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(mRenderer, rc);
}

void CGame::fpsChanged( int fps ) {
	std::stringstream ss;
	ss << "Happy Orc " << fps << " FPS";
	SDL_SetWindowTitle(mWindow, ss.str().c_str());
}

void CGame::onQuit() {
	fprintf(stdout, "Good bye!\n");
}

void CGame::update(double deltaTime) {
	//fprintf(stderr, "dt: %04f\n", deltaTime);
    if ( keys[SDLK_LEFT] ) {
        //hero.x -= HERO_SPEED ;
		ahero.moveLeft(deltaTime);
		//hero.update_left();
    } else if ( keys[SDLK_RIGHT] ) {
        //hero.x += HERO_SPEED ;
		ahero.moveRight(deltaTime);
		//hero.update_right();
    }
//    fprintf(stderr, "ham: %d %d\n", aham.getX(), aham.getY());

    if (masterdata.direction == 0) {
    	amaster.moveLeft(deltaTime);
    } else if (masterdata.direction == 1) {
    	amaster.moveRight(deltaTime);
    }

    if (masterdata.dropham) {
    	aham.moveDown(deltaTime);
    }

    if (aham.getY() + HAM_WIDTH >= DISPLAY_HEIGHT*0.9)
    {
    	masterdata.dropham = false;
    	onLoose();
    }

    checkCollisions();
}

void CGame::onKeyDown( SDL_Event* evt ) {
    keys[ evt->key.keysym.sym ] = 1 ;
}

void CGame::onKeyUp( SDL_Event* evt ) {
    keys[ evt->key.keysym.sym ] = 0 ;
}

void CGame::setKeyColor(SDL_Surface* surface)
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

void CGame::loadsprite(const char* path, SDL_Texture*& texture)
{
	SDL_Surface* temp = SDL_LoadBMP(path);
	if (temp)
	{
		setKeyColor(temp);
		texture = SDL_CreateTextureFromSurface(mRenderer, temp);
		SDL_FreeSurface(temp);
	} else {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}
}

void CGame::checkCollisions() {
	using framework::math::COverlapTester;
	if (COverlapTester::overlapRectangles(ahero.getBound(), aham.getBound()))
	{
//		fprintf(stderr, "Overlaps\n");
		mscorepoints += 1;

		// reset aham positions
		aham.setY(-HAM_WIDTH);
		masterdata.dropham = false;

		if (mscorepoints % 20 == 0 && aham.getSpeed() < 10) {
			aham.setSpeed(aham.getSpeed() + static_cast<uint32_t>(mscorepoints / 20));
		}
	}
}

void CGame::onLoose() {
	fprintf(stdout, "You loose with %d PTS. Have a nice day!\n", mscorepoints);
}

