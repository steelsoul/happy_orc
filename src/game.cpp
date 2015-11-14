
#include "CBaseSprite.hpp"
#include "COrcSprite.hpp"

#include "CMainDispatcher.hpp"
#include <iostream>
#include "CGameScreen.hpp"
#include "CMenuScreen.hpp"

#include "settings.hpp"

using namespace std;

using namespace happyorc;

#ifdef _WIN32
int wmain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return -4;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH,	DISPLAY_HEIGHT,
			SDL_WINDOW_HIDDEN /*| SDL_WINDOW_FULLSCREEN*/, &window,
									&renderer) < 0) return -3;

	SDL_ShowWindow(window);

	if (0 != TTF_Init()) return -1;

	TTF_Font* commonFont = TTF_OpenFont("Effinground.ttf", 30);
	if (!commonFont) return -2;

	TTF_SetFontStyle(commonFont, TTF_STYLE_NORMAL | TTF_STYLE_BOLD);
	TTF_SetFontOutline(commonFont, 0);
	TTF_SetFontKerning(commonFont, 0);
	TTF_SetFontHinting(commonFont, TTF_HINTING_NORMAL);

	cerr << "TTF library initialized.\n";

	CMainDispatcher dispatcher(window, renderer, commonFont);
	CMenuScreen menu(dispatcher, commonFont);

	dispatcher.setNewPlayable(&menu);
	dispatcher.play();

	TTF_CloseFont(commonFont);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    SDL_Quit();

	return 0;
}
