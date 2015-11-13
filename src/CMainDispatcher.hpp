
#ifndef SRC_CMAINDISPATCHER_HPP_
#define SRC_CMAINDISPATCHER_HPP_

#include <SDL.h>

#include "ICompleteListener.hpp"
#include "IDestroyListener.hpp"

#include "IPlayable.hpp"

#include "CGame.hpp"
#include "CMenu.hpp"

#include <vector>

class CMainDispatcher: public ICompleteListener, public IDestroyListener {
public:
	CMainDispatcher(TTF_Font* font);
	virtual ~CMainDispatcher();

	void setNewPlayable(IPlayable* playable);
	void play();
	void quit();

	/**
	 * ICompleteListener interface
	 * */
	void onPrepare(int percentage) override;
	void onComplete(IPlayable* playable) override;

	/**
	 * IDestroyListener interface
	 * */
	void onDestroy(IPlayable* which) override;

private:
	bool initSDL();
	void shutdownSDL();

	void switchPlayable();


private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
	IPlayable* mNewPlayable = nullptr;
	IPlayable* mToRemovePlayable = nullptr;
	std::vector<IPlayable*> mPlayables;
	bool mQuit = false;

};

#endif /* SRC_CMAINDISPATCHER_HPP_ */
