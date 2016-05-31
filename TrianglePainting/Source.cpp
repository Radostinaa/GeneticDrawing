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

	sdl.drawTirangle({ { 10,10 },{ 20, 20 },{ 100,20 } }, { 255, 0, 0 });

	sdl.update();
	while (1) { SDL_Delay(1); }
	return 0;
}