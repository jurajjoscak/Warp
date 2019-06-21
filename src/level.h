#include "player.h"
#include <string>
using namespace std;

///////////////////////////////////
// Level zah��a hr��a, geometriu
// a FFD mrie�ku
///////////////////////////////////
class WPLevel
{
	public:
		WPWarpable geometry;
		WPSpace grid;
		WPPlayer player;
		WPLevel();
		WPLevel(string levelname);
		int run();      // Vypo��ta jednu iter�ciu hernej slu�ky
};