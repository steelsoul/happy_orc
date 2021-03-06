/*
 * CHallOfFameScreen.hpp
 *
 *  Created on: Nov 14, 2015
 *      Author: alexander
 */

#ifndef SRC_CHALLOFFAMESCREEN_HPP_
#define SRC_CHALLOFFAMESCREEN_HPP_

#include "CBaseScreen.hpp"

class CHallOfFameScreen: public CBaseScreen {
public:
	static const char* const HALLOFFAME_FILE_NAME;
	static const int DEFAULT_SCORE;

	CHallOfFameScreen(CMainDispatcher& dispatcher, TTF_Font* font);
	virtual ~CHallOfFameScreen();

	/**
		IPlayable interface
	*/
	void init(SDL_Renderer* renderer) override;
	bool run(SDL_Window* window, SDL_Renderer* renderer, double dt) override;
	void onPrepare(int perc) override;
	void cleanup(IPlayable* playable) override;

	static void writeScorePoints(int score);

protected:
	void onKeyDown(const SDL_Event& event);
	void onKeyUp(const SDL_Event& event);
	void drawFrame(SDL_Window* window, SDL_Renderer* renderer);

private:
	static void updateHallOfFame(std::vector<int>& data, int score);

private:
	TTF_Font* mFont;
	SDL_Texture* mBestTexture;
	std::vector<int> mBest;
};

#endif /* SRC_CHALLOFFAMESCREEN_HPP_ */
