#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"
#include "Image.h"

int MaxTriangles = 5;
int genSize = 20;
SDLWrapper sdl(900, 500);

int main(int argc, char *argv[])
{
	if (!sdl.initSDL())
	{
		return -1;
	}

	SDL_Surface* original = sdl.drawImageFromPath("../Images/cookie_monster.png");

	Image image(original->w, original->h);

	Random rnd;
	for (int i = 0; i < MaxTriangles; i++)
	{
		Triangle tr = rnd.getRandomTriangle(450, 450);
		image.triangles.push_back(tr);
	}
	image.getImage();
	sdl.drawImage(image);

	//for (int i = 0; i < MaxTriangles; i++)
	//{
	//	sdl.drawTirangle(rnd.getRandomTriangle(450, 450));
	//}
	//

	while (1) { SDL_Delay(1); }
	return 0;
}

