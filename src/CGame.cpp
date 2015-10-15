/*
 * CCGame.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#include "CGame.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "settings.hpp"

namespace happyorc {

CGame::CGame()
: keys()
, frameSkip(0u)
, running(0)
, window(NULL)
, renderer(NULL)
, background(NULL)
, orcs(NULL)
, ham(NULL)
, ahero(0, 0, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH, 3)
, aham(rand() % (DISPLAY_WIDTH - HAM_WIDTH), -HAM_WIDTH, HAM_WIDTH, HAM_HEIGHT, HAM_SPEED, DISPLAY_HEIGHT)
{
	srand(time(NULL));
	aham.setX(rand() % (DISPLAY_WIDTH - HAM_WIDTH));
}

CGame::~CGame() {
    this->stop();
}

void CGame::start() {
	int flags = SDL_WINDOW_HIDDEN/* | SDL_WINDOW_FULLSCREEN*/ ;
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		return;
	}
	if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &window, &renderer)) {
		return;
	}
	const char* filename = "forest_480x320.jpg";

	background = IMG_LoadTexture(renderer, filename);
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

	ahero.setY(DISPLAY_HEIGHT*(1 - 0.2));

	this->running = 1;
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

    CBaseSprite::CRect spriteSrc = ahero.getSrcRect();
    aSrcRect.x = spriteSrc.x;
    aSrcRect.y = spriteSrc.y;
    aSrcRect.w = spriteSrc.w;
    aSrcRect.h = spriteSrc.h;

	SDL_RenderCopy(renderer, orcs, &aSrcRect, &aDstRect);

	/// Render ham
	aDstRect.x = aham.getX();
	aDstRect.y = aham.getY();
	aDstRect.w = HAM_WIDTH;
	aDstRect.h = HAM_HEIGHT;

	spriteSrc = aham.getSrcRect();
    aSrcRect.x = spriteSrc.x;
    aSrcRect.y = spriteSrc.y;
    aSrcRect.w = spriteSrc.w;
    aSrcRect.h = spriteSrc.h;

	SDL_RenderCopy(renderer, ham, &aSrcRect, &aDstRect);

	SDL_RenderPresent(renderer);
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
	fprintf(stderr, "Good bye!\n");
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
            update();
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

void CGame::update() {
    if ( keys[SDLK_LEFT] ) {
        //hero.x -= HERO_SPEED ;
		ahero.moveLeft();
		//hero.update_left();
    } else if ( keys[SDLK_RIGHT] ) {
        //hero.x += HERO_SPEED ;
		ahero.moveRight();
		//hero.update_right();
    } /*else if ( keys[SDLK_UP] ) {
        hero.y -= HERO_SPEED ;
    } else if ( keys[SDLK_DOWN] ) {
        hero.y += HERO_SPEED ;
    }*/

    fprintf(stderr, "ham: %d %d\n", aham.getX(), aham.getY());
    aham.moveDown();
    if (aham.getY() + HAM_WIDTH >= DISPLAY_HEIGHT*0.9)
    {
    	aham.setX(rand() % (DISPLAY_WIDTH - HAM_WIDTH));
    	aham.setY(-HAM_WIDTH);
    }
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

} /* namespace happyorc */
