
#include "CBaseSprite.hpp"
#include "COrcSprite.hpp"

#include "CMainDispatcher.hpp"
#include <iostream>
#include "CGameScreen.hpp"
#include "CMenuScreen.hpp"

using namespace std;

using namespace happyorc;

#ifdef _WIN32
int wmain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	// TODO: make sure that SDL library is initialized only once and
	// it is closed only once too.

	if (0 != TTF_Init()) return -1;

	TTF_Font* commonFont = TTF_OpenFont("Effinground.ttf", 30);
	if (!commonFont) return -2;

	TTF_SetFontStyle(commonFont, TTF_STYLE_NORMAL | TTF_STYLE_BOLD);
	TTF_SetFontOutline(commonFont, 0);
	TTF_SetFontKerning(commonFont, 0);
	TTF_SetFontHinting(commonFont, TTF_HINTING_NORMAL);

	cerr << "TTF library initialized.\n";

	CMainDispatcher dispatcher(commonFont);
	CMenuScreen menu(dispatcher, commonFont);

	dispatcher.setNewPlayable(&menu);
	dispatcher.play();

	TTF_CloseFont(commonFont);
	TTF_Quit();

	return 0;
}
