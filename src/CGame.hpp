/*
 * CGame.hpp
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CGAME_HPP_
#define SRC_CGAME_HPP_

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include "COrcSprite.hpp"
#include "CHamSprite.hpp"

namespace happyorc {

class CGame {
public:
	CGame();
	virtual ~CGame();

    void start();
private:

    void stop();
    void draw();
    void fillRect(SDL_Rect* rc, int r, int g, int b );
    void fpsChanged( int fps );
    void onQuit();
    void onKeyDown( SDL_Event* event );
    void onKeyUp( SDL_Event* event );
    void run();
    void update();

	void loadsprite(const char* path, SDL_Texture*&);
	void setKeyColor(SDL_Surface*);

private:
    std::map<int,int> keys;
    Uint32 frameSkip ;
    int running ;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* background;
	SDL_Texture* orcs;
	SDL_Texture* ham;

	//TTF_Font* mfont;

    COrcSprite ahero;
	CHamSprite aham;
};

} /* namespace happyorc */

#endif /* SRC_CGAME_HPP_ */
