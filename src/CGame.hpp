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

#include "IPlayable.hpp"

class CMainDispatcher;

class CGame: public IPlayable {
	struct MasterData {
		MasterData()
		: direction(2), dropham(false)
		{}
		int direction;
		bool dropham;
	};
public:
	CGame(CMainDispatcher&, TTF_Font*);
	virtual ~CGame();
	void startTimer();

	/**
		IPlayable interface
	*/
	virtual void init(SDL_Renderer* renderer) override;
	virtual bool run(SDL_Window* window, SDL_Renderer* renderer) override;
	virtual void onPrepare(int perc) override;
	virtual bool isAlive() const override;
	virtual void cleanup(IPlayable* playable) override;

private:

    void stop();
    void draw(SDL_Window* window, SDL_Renderer* renderer);
    void fillRect(SDL_Renderer* renderer, SDL_Rect* rc, int r, int g, int b );
    void fpsChanged( int fps );
    void onQuit();
    void onKeyDown( SDL_Event* event );
    void onKeyUp( SDL_Event* event );
    void update(double deltaTime);

	void loadsprite(SDL_Renderer* renderer, const char* path, SDL_Texture*&);
	void setKeyColor(SDL_Surface*);

	void drawScore(SDL_Renderer* renderer);

	void checkCollisions();
	void onLoose();

private:
	CMainDispatcher& mDispatcher;
    std::map<int,int> keys;
    Uint32 frameSkip ;
	bool mpaused;

    SDL_Texture* background;
	SDL_Texture* orcs;
	SDL_Texture* ham;

	TTF_Font* mFont;
	SDL_Texture* mscore;
	int mscorepoints;

	SDL_Texture* paused;

    happyorc::COrcSprite ahero;
	happyorc::CHamSprite aham;
	happyorc::COrcSprite amaster;
	MasterData masterdata;
	bool mAlive;

	SDL_TimerID mTimerID;
};


#endif /* SRC_CGAME_HPP_ */
