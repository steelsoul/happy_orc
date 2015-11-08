
#include "CBaseSprite.hpp"
#include "CGame.hpp"
#include "COrcSprite.hpp"

#include "CMainDispatcher.hpp"
#include "CMenu.hpp"

using namespace happyorc;

#ifdef _WIN32
int wmain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	CMainDispatcher dispatcher;
	CMenu menu(dispatcher);

	dispatcher.setNewPlayable(&menu);
	dispatcher.play();

	return 0;
}
