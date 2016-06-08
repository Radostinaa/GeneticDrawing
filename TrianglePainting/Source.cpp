#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"
#include "Image.h"
#include "GA.h"

std::vector<Image> generation;

void firstGeneration(int width, int height, Random& rnd, int size)
{
	for (int c = 0; c < size; ++c){

		Image image(width, height);

		for (int i = 0; i < size; i++)
		{
			Triangle tr = rnd.getRandomTriangle(450, 450);
			image.triangles.push_back(tr);
		}

		image.generatePixels();

		generation.push_back(std::move(image));
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
	SDLWrapper sdl(900, 500);
	if (!sdl.initSDL())
	{
		return -1;
	}

	int MaxTriangles = 20;
	int MaxGenSize = 20;
	Random rnd;

	SDL_Surface* original = sdl.drawImageFromPath("../Images/cookie_monster.png");

	GA ga(MaxTriangles, original);

	firstGeneration(original->w, original->h, rnd, MaxGenSize);

	while (!sdl.Quit())
	{
		sdl.clear();
		sdl.drawImage(original, true);

		//get fitness
		for (auto &img : generation)
		{
			ga.fitness(img);
		}

		sotrGeneration();

		//draw first image
		sdl.drawImage(generation[0].surface, false);

		std::vector<Image> newGen;

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

		//add the best ones
		for (int i = 0; i < MaxGenSize/10; i++)
		{
			newGen.push_back(std::move(generation[i]));
		}

		generation = std::move(newGen);

		sdl.checkForEvent();

	}

	return 0;
}


