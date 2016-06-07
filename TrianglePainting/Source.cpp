#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"
#include "Image.h"

int MaxTriangles = 5;
int MaxGenSize = 5;
int genSize = 20;
SDLWrapper sdl(900, 500);
Random rnd;

std::vector<Image> generation;
SDL_Surface* original;

void firstGeneration()
{
	for (int c = 0; c < MaxGenSize; ++c){

		Image image(original->w, original->h);

		for (int i = 0; i < MaxTriangles; i++)
		{
			Triangle tr = rnd.getRandomTriangle(450, 450);
			image.triangles.push_back(tr);
		}

		image.getImage();

		generation.push_back(image);
	}
}

int main(int argc, char *argv[])
{
	if (!sdl.initSDL())
	{
		return -1;
	}

	original = sdl.drawImageFromPath("../Images/cookie_monster.png");

	firstGeneration();

	/*while (1)
	{*/
		//draw first image
		sdl.drawImage(generation[0]);
		


	//}


	//while (1) { SDL_Delay(1); }
	return 0;
}


