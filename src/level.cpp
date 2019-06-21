#include "level.h"

WPLevel::WPLevel() :
	geometry("untitled"),
	grid(0.5, &geometry),
	player(&grid)
{}

WPLevel::WPLevel(string levelname) :
	geometry(levelname),
	grid(0.5, &geometry),
	player(&grid)
{}

int WPLevel::run()
{
	int re = player.control();

	player.update();

	if (player.outOfBounds())
		return 0;

	if ((player.main_camera.getPosition() - geometry.finish).length() < 2.0 && geometry.fadeout < 1.0)  // Keï je hráè blízko cie¾a, ukonèi level
		geometry.fadeout = 2.0;
		//return 2;

	if (geometry.fadeout > 0)
	{
		geometry.fadeout -= 0.01;
		if(geometry.fadeout < 1.02 && geometry.fadeout > 1.0)
			return 2;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	player.draw();

	//std::cout << std::fixed << std::setprecision(6) << geometry.fadeout << "\n";

	return re;
}