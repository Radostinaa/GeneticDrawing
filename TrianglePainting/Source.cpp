#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"
#include "Image.h"
#include "GA.h"

int MaxTriangles = 5;
int MaxGenSize = 20;
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

bool operator < (const Image& a, const Image& b)
{
	return a.fitness < b.fitness;
}

void sotrGeneration()
{
	std::sort(generation.begin(), generation.end());
}

int main(int argc, char *argv[])
{
	if (!sdl.initSDL())
	{
		return -1;
	}

	original = sdl.drawImageFromPath("../Images/cookie_monster.png");

	GA ga(MaxTriangles, original);

	firstGeneration();

	//
	while (!sdl.Quit())
	{
		//get fitness
		for (auto &img : generation)
		{
			ga.fitness(img);
		}

		sotrGeneration();

		//draw first image
		sdl.drawImage(generation[0]);

		std::vector<Image> newGen;

		//add the best ones
		for (int i = 0; i < MaxGenSize/10; i++)
		{
			newGen.push_back(generation[i]);
		}

		//cross
		int m, f;
		for (int i = MaxGenSize / 10; i < MaxGenSize; i++)
		{
			m = rnd.getImageIndex(1, MaxGenSize - 1);
			f = rnd.getImageIndex(0, m - 1);
			Vector2 from(0,0), to(0,0);
			newGen.push_back(ga.cross(generation[m], generation[f]));

			//sdl.drawLine(from, to, Color(255, 0, 0, 255));
		}

		//mutate
		for (int i = 0; i < MaxGenSize/5; i++)
		{
		}

		generation = newGen;
		sdl.checkForEvent();

	}


	//while (1) { SDL_Delay(1); }
	return 0;
}


