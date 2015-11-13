/*
 * COptions.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: alexander
 */

#ifndef SRC_COPTIONS_HPP_
#define SRC_COPTIONS_HPP_

#include "CBaseScreen.hpp"

class COptions: public CBaseScreen {
public:
	COptions(CMainDispatcher&, TTF_Font*);
	virtual ~COptions();

	/**
	 * IPlayable interface
	 * */
	void init(SDL_Renderer* renderer) override;
	bool run(SDL_Window* window, SDL_Renderer* renderer) override;
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

#endif /* SRC_COPTIONS_HPP_ */
