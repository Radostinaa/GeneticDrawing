#include "SDLWrapper.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	SDLWrapper sdl(700, 700);
	if (!sdl.initSDL())
	{
		return -1;
	}

	sdl.drawImage("../Images/cookie_monster.png");
	sdl.update();
	return 0;
}