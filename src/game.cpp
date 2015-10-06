/*
 SDL2 example that allows user to move an object using arrow keys.
 This is meant to be used as a convenient single-file starting point for
 more complex projects.
 
 Author: Andrew Lim Chong Liang
 windrealm.org
 */
#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <map>

enum {
    DISPLAY_WIDTH  = 480
    , DISPLAY_HEIGHT = 320
    , UPDATE_INTERVAL = 1000/60
    , HERO_SPEED = 2
};

class BaseSprite {
public: int x, y;
		BaseSprite() : x(0), y(0) {}
};

class Sprite: public BaseSprite {
public:
    Sprite()
		: BaseSprite()
		, mStates(0)
		, mState(0)
		, mWidth(0)
		, mHeight(0)
	{
	}
	
	Sprite(int states, int width, int height)
		: BaseSprite()
		, mStates(states)
		, mState(0)
		, mWidth(width)
		, mHeight(height)
	{
	}

	void update_right()
	{
		if (x % 20 == 0)
		{
			mState++;
			if (mStates < mState)
			{
				mState = 0;
			}
		}
	}

	void update_left()
	{
		if (x % 20 == 0)
		{
			mState--;
			if (0 > mState)
			{
				mState = mStates;
			}
		}
	}

	SDL_Rect getSrcRect()
	{
		SDL_Rect result;
		result.x = mState * mWidth;
		result.y = 0;
		result.w = mWidth;
		result.h = mHeight;

		return result;
	}

private:
	int mStates, mState, mWidth, mHeight;
};

class Game {
public:
    Game();
    ~Game();
    void start();
    void stop() ;
    void draw();
    void fillRect(SDL_Rect* rc, int r, int g, int b );
    void fpsChanged( int fps );
    void onQuit();
    void onKeyDown( SDL_Event* event );
    void onKeyUp( SDL_Event* event );
    void run();
    void update();
	void loadorc(const char* path);
private:
    std::map<int,int> keys; // No SDLK_LAST. SDL2 migration guide suggests std::map
    Uint32 frameSkip ;
    int running ;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Sprite hero ;
    SDL_Texture* background;
	SDL_Texture* orcs;
};

Game::Game()
:frameSkip(0u), running(0), window(NULL), renderer(NULL), hero(3, 32, 32), background(0), orcs(0) {
}

Game::~Game() {
    this->stop();
}

void Game::start() {
	int flags = SDL_WINDOW_HIDDEN /*| SDL_WINDOW_FULLSCREEN */;
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

	this->running = 1;
	SDL_ShowWindow(window);
	run();
}

void Game::draw() {
	SDL_Rect heroRect;

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (NULL != background) {
		SDL_RenderCopy(renderer, background, NULL, NULL);
	}

	SDL_Rect src_rect;
	src_rect.x = src_rect.y = 0;
	src_rect.h = src_rect.w = 32;

	//// Render hero
	heroRect.x = hero.x;
	heroRect.y = hero.y;
	heroRect.w = 32;
	heroRect.h = 32;
	SDL_RenderCopy(renderer, orcs, &hero.getSrcRect(), &heroRect);
	//fillRect(&heroRect, 255, 0, 0 );

	SDL_RenderPresent(renderer);
}

void Game::stop() {
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

void Game::fillRect(SDL_Rect* rc, int r, int g, int b ) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void Game::fpsChanged( int fps ) {
    char szFps[ 128 ] ;
    sprintf_s( szFps, "%s: %d FPS", "SDL2 Base C++ - Use Arrow Keys to Move", fps );
    SDL_SetWindowTitle(window, szFps);
}

void Game::onQuit() {
    running = 0 ;
}

void Game::run() {
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

void Game::update() {
    if ( keys[SDLK_LEFT] ) {
        hero.x -= HERO_SPEED ;
		hero.update_left();
    } else if ( keys[SDLK_RIGHT] ) {
        hero.x += HERO_SPEED ;
		hero.update_right();
    } else if ( keys[SDLK_UP] ) {
        hero.y -= HERO_SPEED ;
    } else if ( keys[SDLK_DOWN] ) {
        hero.y += HERO_SPEED ;
    }
}

void Game::onKeyDown( SDL_Event* evt ) {
    keys[ evt->key.keysym.sym ] = 1 ;
}

void Game::onKeyUp( SDL_Event* evt ) {
    keys[ evt->key.keysym.sym ] = 0 ;
}

void Game::loadorc(const char* path)
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

#ifdef _WIN32
int wmain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	Game game;
	game.start();
	return 0;
}
