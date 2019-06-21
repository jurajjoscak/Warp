#include "player.h"
#include <string>
using namespace std;

///////////////////////////////////
// Level zahàòa hráèa, geometriu
// a FFD mriežku
///////////////////////////////////
class WPLevel
{
	public:
		WPWarpable geometry;
		WPSpace grid;
		WPPlayer player;
		WPLevel();
		WPLevel(string levelname);
		int run();      // Vypoèíta jednu iteráciu hernej sluèky
};