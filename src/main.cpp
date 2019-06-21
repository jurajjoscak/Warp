#include <iostream>
#include "level.h"
using namespace std;

SDL_Window *mainWindow;
SDL_GLContext mainContext;
SDL_Surface * menuscreen;
SDL_Surface * cleared;

void __stdcall debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, void * userParam)
{
	cout << message << endl;
}


int init()
{
    SDL_Init(SDL_INIT_VIDEO);
    mainWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	mainContext = SDL_GL_CreateContext(mainWindow);
	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, NULL, GL_FALSE);
	glDebugMessageCallback((GLDEBUGPROC)debugMessageCallback, NULL);

	menuscreen = SDL_LoadBMP("resources/textures/menu.bmp");
	cleared = SDL_LoadBMP("resources/textures/congrats.bmp");
    return 0;
}

int startLevel(string levelname)
{
	SDL_ShowCursor(0);
	WPLevel *level = new WPLevel(levelname);

	SDL_WarpMouseInWindow(mainWindow, SCREEN_W / 2, SCREEN_H / 2);
	while (true)
	{
		int state = level->run();

		if (state == 0)
		{
			SDL_ShowCursor(1);
			return 0;
		}
		if (state == 2)
		{
			SDL_ShowCursor(1);
			return 1;
		}

		SDL_WarpMouseInWindow(mainWindow, SCREEN_W / 2, SCREEN_H / 2);
		SDL_GL_SwapWindow(mainWindow);
	}
}

void Quit()
{
	SDL_GL_DeleteContext(mainContext);
	SDL_GL_SwapWindow(mainWindow);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}

int menu()
{
	SDL_BlitSurface(menuscreen, NULL, SDL_GetWindowSurface(mainWindow), NULL);
	SDL_UpdateWindowSurface(mainWindow);

	Uint32 mouse_state;

	int x, y;
	while (true)
	{
		SDL_PumpEvents();
		mouse_state = SDL_GetMouseState(&x, &y);
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			if (x > 450 && x < 1090)
			{
				if (y > 30 && y < 240)
					return 1;
				if (y > 240 && y < 430)
					return 2;
				if (y > 430 && y < 630)
					return 3;
			}
			if (x > 30 && x < 380 && y > 570 && y < 730)
				return 0;
		}
	}
	return 0;
	/*return 1;
	return 2;
	return 3;*/
}

void congrats()
{
	SDL_BlitSurface(cleared, NULL, SDL_GetWindowSurface(mainWindow), NULL);
	SDL_UpdateWindowSurface(mainWindow);

	SDL_Delay(3000);
}

int main(int argc, char **argv)
{
    init();
	
	menu:
	switch (menu())
	{
		case 0: break;
		case 1:	if (startLevel("first"))
					congrats();
				goto menu;
		case 2:	if (startLevel("untitled"))
					congrats();
				goto menu;
		case 3: if (startLevel("hallway"))
						congrats();
				goto menu;

	}

	Quit();

    return 0;
}
