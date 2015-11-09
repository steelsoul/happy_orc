
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
	CMainDispatcher();
	virtual ~CMainDispatcher();

	void setNewPlayable(IPlayable* playable);
	void play();
	void quit();

	/**
	 * ICompleteListener interface
	 * */
	void onPrepare(int percentage) override;
	void onComplete() override;

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

	std::vector<IPlayable*> mPlayables;
	bool mPrepared;
	bool mIsRunning;
	bool mRemoved;

};

#endif /* SRC_CMAINDISPATCHER_HPP_ */
