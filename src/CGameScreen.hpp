/*
 * CGameScreen.hpp
 * game screen implementation.
 *
 *  Created on: Oct 7, 2015
 *      Author: alexander
 */

#ifndef SRC_CGAMESCREEN_HPP_
#define SRC_CGAMESCREEN_HPP_

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>

#include "CBaseScreen.hpp"
#include "COrcSprite.hpp"
#include "CHamSprite.hpp"

#include "IPlayable.hpp"

class CMainDispatcher;

class CGameScreen: public CBaseScreen {
	struct MasterData {
		MasterData()
		: direction(2), dropham(false)
		{}
		int direction;
		bool dropham;
	};
public:
	CGameScreen(CMainDispatcher&, TTF_Font*);
	virtual ~CGameScreen();
	void startTimer();

	/**
		IPlayable interface
	*/
	void init(SDL_Renderer* renderer) override;
	bool run(SDL_Window* window, SDL_Renderer* renderer, double dt) override;
	void onPrepare(int perc) override;
	void cleanup(IPlayable* playable) override;

protected:
	void onKeyDown(const SDL_Event& event) override;
	void onKeyUp(const SDL_Event& event) override;
    void drawFrame(SDL_Window* window, SDL_Renderer* renderer) override;

private:

    void stop();

    void fillRect(SDL_Renderer* renderer, SDL_Rect* rc, int r, int g, int b );
    void fpsChanged( int fps );
    void onQuit();
    void update(double deltaTime);

	void loadsprite(SDL_Renderer* renderer, const char* path, SDL_Texture*&);
	void setKeyColor(SDL_Surface*);

	void drawScore(SDL_Renderer* renderer);

	void checkCollisions();
	void onLoose();

private:
	CMainDispatcher& mDispatcher;

	int mPauseState;

    SDL_Texture* mBgTexture;
	SDL_Texture* mOrcTexure;
	SDL_Texture* mHamTexture;

	SDL_Texture* mScoreTexture;
	int mScorepoints;
	bool mUpdateScores;

	SDL_Texture* mPausedTexture;
	TTF_Font* mFont;

    happyorc::COrcSprite mAhero;
	happyorc::CHamSprite mAham;
	happyorc::COrcSprite mAmaster;
	MasterData mMasterdata;
	int mTimerID2;
	std::map<SDL_Keycode, bool> mInput;
};


#endif /* SRC_CGAMESCREEN_HPP_ */
