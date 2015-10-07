
#include "CBaseSprite.hpp"
#include "CSprite.hpp"
#include "CGame.hpp"


#ifdef _WIN32
int wmain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	happyorc::CGame game;
	game.start();
	return 0;
}
