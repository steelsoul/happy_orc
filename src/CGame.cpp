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
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "settings.hpp"

#include "overlaptester/COverlapTester.hpp"

namespace happyorc {

Uint32 my_timer_fn(Uint32 interval, void *param)
{
//	fprintf(stderr, "== on Time () ==\n");
	if (param) {
		CGame* game = reinterpret_cast<CGame*>(param);
		game->startTimer();
	}
	return 0u;
}

CGame::CGame()
: keys()
, frameSkip(0u)
, running(0)
, window(NULL)
, renderer(NULL)
, background(NULL)
, orcs(NULL)
, ham(NULL)
, mfont(NULL)
, mscore(NULL)
, mscorepoints(0)
, ahero(0, 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, aham(rand() % (DISPLAY_WIDTH - HAM_WIDTH), -HAM_WIDTH, HAM_WIDTH, HAM_HEIGHT, HAM_SPEED, DISPLAY_HEIGHT)
, amaster(rand() % (DISPLAY_WIDTH - ORC_WIDTH), 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, masterdata()
{
	srand(time(NULL));
	aham.setX(rand() % (DISPLAY_WIDTH - HAM_WIDTH));
}

void CGame::startTimer() {
	SDL_TimerID tid = SDL_AddTimer(500 + rand() % 800, my_timer_fn, this);
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

CGame::~CGame() {
    this->stop();
}


void CGame::start() {
	int flags = SDL_WINDOW_HIDDEN/* | SDL_WINDOW_FULLSCREEN*/ ;
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}
	/* Initialize the TTF library */
	if (TTF_Init() < 0) {
		fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
		return;
	}
	if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &window, &renderer)) {
		return;
	}

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

	mfont = TTF_OpenFont("Effinground.ttf", 30);
	if (!mfont) {
		fprintf(stderr, "Error: %s", SDL_GetError());
		return;
	}

	{
		int renderStyle = TTF_STYLE_NORMAL | TTF_STYLE_BOLD;
		int hinting = TTF_HINTING_NORMAL;
		int outline = 0;
		int kerning = 0;

		TTF_SetFontStyle(mfont, renderStyle);
		TTF_SetFontOutline(mfont, outline);
		TTF_SetFontKerning(mfont, kerning);
		TTF_SetFontHinting(mfont, hinting);
	}



	ahero.setY(static_cast<int>(DISPLAY_HEIGHT*(1 - 0.2)));

	this->running = 1;
	startTimer();

	SDL_ShowWindow(window);
	run();
}

void CGame::draw() {
	SDL_Rect aDstRect;
	SDL_Rect aSrcRect;

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (NULL != background) {
		SDL_RenderCopy(renderer, background, NULL, NULL);
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

	SDL_RenderCopy(renderer, orcs, &aSrcRect, &aDstRect);

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

	SDL_RenderCopy(renderer, orcs, &aSrcRect, &aDstRect);

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

		SDL_RenderCopy(renderer, ham, &aSrcRect, &aDstRect);
	}
	drawScore();

	SDL_RenderPresent(renderer);
}

void CGame::drawScore() {
	SDL_Color textColor = { 0xFF, 0x00, 0x00, 0 };
	std::stringstream ss;
	ss << mscorepoints << " PTS";
	SDL_Surface * tempsurf = TTF_RenderText_Solid(mfont, ss.str().c_str(), textColor);

	if (mscore) {
		SDL_DestroyTexture(mscore);
		mscore = NULL;
	}

	mscore = SDL_CreateTextureFromSurface(renderer, tempsurf);

	SDL_Rect textsource;

	textsource.x = 0;
	textsource.y = 0;
	textsource.w = tempsurf->w; 
	textsource.h = tempsurf->h;

	SDL_FreeSurface(tempsurf);

	SDL_Rect dst;
	dst.x = 10; dst.y = 50; dst.w = textsource.w; dst.h = textsource.h;
	SDL_RenderCopy(renderer, mscore, &textsource, &dst);
}


void CGame::stop() {
    if (NULL != renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (NULL != window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    if (NULL != background) {
    	SDL_DestroyTexture(background);
    }
	if (NULL != orcs) {
		SDL_DestroyTexture(orcs);
	}
	if (NULL != ham) {
		SDL_DestroyTexture(ham);
	}
	if (NULL != mscore) {
		SDL_DestroyTexture(mscore);
	}

	TTF_CloseFont(mfont);
	TTF_Quit();
    SDL_Quit() ;
}

void CGame::fillRect(SDL_Rect* rc, int r, int g, int b ) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void CGame::fpsChanged( int fps ) {
	std::stringstream ss;
	ss << "Happy Orc " << fps << " FPS";
	SDL_SetWindowTitle(window, ss.str().c_str());
}

void CGame::onQuit() {
	fprintf(stdout, "Good bye!\n");
    running = 0 ;
}

void CGame::run() {
	Uint32 past = SDL_GetTicks();
    Uint32 now = past, pastFps = past ;
    Uint32 fps = 0, framesSkipped = 0 ;
    SDL_Event event ;
    while ( running ) {
        int timeElapsed = 0 ;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:    onQuit();            break;
                case SDL_KEYDOWN: onKeyDown( &event ); break ;
                case SDL_KEYUP:   onKeyUp( &event );   break ;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEMOTION:
                    break ;
            }
        }
        // update/draw
        timeElapsed = (now=SDL_GetTicks()) - past ;
        if ( timeElapsed >= UPDATE_INTERVAL  ) {
            past = now ;
            update(timeElapsed/10.0);
            if ( framesSkipped++ >= frameSkip ) {
                draw();
                ++fps ;
                framesSkipped = 0 ;
            }
        }
        // fps
        if ( now - pastFps >= 1000 ) {
            pastFps = now ;
            fpsChanged( fps );
            fps = 0 ;
        }
        // sleep?
        SDL_Delay( 1 );
    }
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
		texture = SDL_CreateTextureFromSurface(renderer, temp);
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
	running = 0;
	fprintf(stdout, "You loose with %d PTS. Have a nice day!\n", mscorepoints);
}

} /* namespace happyorc */
