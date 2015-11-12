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
	COptions(CMainDispatcher& );
	virtual ~COptions();

	/**
	 * IPlayable interface
	 * */
	void init(SDL_Window* window, SDL_Renderer* renderer) override;
	bool run() override;
	void onPrepare(int perc) override;
	bool isAlive() const override;
	void cleanup() override;

private:
	void drawFrame() override;

	void readOptionsFromFile();
	void updateOptionsFile();

private:
	bool mSoundEnabled = true;
	bool mMusicEnabled = true;
	int mSelection = 0;

	SDL_Texture* mOptionsTexture = nullptr;

};

#endif /* SRC_COPTIONS_HPP_ */
