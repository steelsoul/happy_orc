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

namespace happyorc {

CGame::CGame()
: frameSkip(0u)
, running(0)
, window(NULL)
, renderer(NULL)
, hero(3, ORC_WIDTH, ORC_HEIGHT, HERO_SPEED, DISPLAY_WIDTH)
, background(0)
, orcs(0)
{
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
	}

	const char* orcimg = "Orcs.bmp";
	loadorc(orcimg);

	hero.y = static_cast<Sint32>(DISPLAY_HEIGHT*(1 - 0.2));

	this->running = 1;
	SDL_ShowWindow(window);
	run();
}

void CGame::draw() {
	SDL_Rect heroRect;
	SDL_Rect srcRect;

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (NULL != background) {
		SDL_RenderCopy(renderer, background, NULL, NULL);
	}

	//// Render hero
	heroRect.x = hero.x;
	heroRect.y = hero.y;
	heroRect.w = ORC_WIDTH;
	heroRect.h = ORC_HEIGHT;

    CSprite::CRect spriteSrc = hero.getSrcRect();
    srcRect.x = spriteSrc.x;
    srcRect.y = spriteSrc.y;
    srcRect.w = spriteSrc.w;
    srcRect.h = spriteSrc.h;

	SDL_RenderCopy(renderer, orcs, &srcRect, &heroRect);

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
		hero.moveLeft();
		//hero.update_left();
    } else if ( keys[SDLK_RIGHT] ) {
        //hero.x += HERO_SPEED ;
		hero.moveRight();
		//hero.update_right();
    } /*else if ( keys[SDLK_UP] ) {
        hero.y -= HERO_SPEED ;
    } else if ( keys[SDLK_DOWN] ) {
        hero.y += HERO_SPEED ;
    }*/
}

void CGame::onKeyDown( SDL_Event* evt ) {
    keys[ evt->key.keysym.sym ] = 1 ;
}

void CGame::onKeyUp( SDL_Event* evt ) {
    keys[ evt->key.keysym.sym ] = 0 ;
}

void CGame::loadorc(const char* path)
{
	SDL_Surface *temp;
	temp = SDL_LoadBMP(path);
	if (temp == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s", path, SDL_GetError());
		// very bad!
	}
	/* Set transparent pixel as the pixel at (0,0) */
	if (temp->format->palette) {
		SDL_SetColorKey(temp, 1, *(Uint8 *)temp->pixels);
	}
	else {
		switch (temp->format->BitsPerPixel) {
		case 15:
			SDL_SetColorKey(temp, 1, (*(Uint16 *)temp->pixels) & 0x00007FFF);
			break;
		case 16:
			SDL_SetColorKey(temp, 1, *(Uint16 *)temp->pixels);
			break;
		case 24:
			SDL_SetColorKey(temp, 1, (*(Uint32 *)temp->pixels) & 0x00FFFFFF);
			break;
		case 32:
			SDL_SetColorKey(temp, 1, *(Uint32 *)temp->pixels);
			break;
		}
	}
	orcs = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
}

} /* namespace happyorc */
