/*
 * COptions.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#ifndef SRC_COPTIONSSCREEN_HPP_
#define SRC_COPTIONSSCREEN_HPP_

#include "CBaseScreen.hpp"

class COptionsScreen: public CBaseScreen {
public:
	COptionsScreen(CMainDispatcher&, TTF_Font*);
	virtual ~COptionsScreen();

	/**
	 * IPlayable interface
	 * */
	void init(SDL_Renderer* renderer) override;
	bool run(SDL_Window* window, SDL_Renderer* renderer, double) override;
	void onPrepare(int perc) override;
	bool isAlive() const override;
	void cleanup(IPlayable* playable) override;

	void onKeyUp(const SDL_Event&) override;
	void onKeyDown(const SDL_Event&) override;

private:
	void drawFrame(SDL_Window* window, SDL_Renderer* renderer) override;

	void readOptionsFromFile();
	void updateOptionsFile();

private:
	bool mSoundEnabled = true;
	bool mMusicEnabled = true;
	int mSelection = 0;
	int mUpdated = true;

	SDL_Texture* mOptionsTexture = nullptr;

};

#endif /* SRC_COPTIONSSCREEN_HPP_ */
