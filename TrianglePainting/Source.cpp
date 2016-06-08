#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"
#include "Image.h"
#include "GA.h"

std::vector<Image> generation;

void firstGeneration(int width, int height, Random& rnd, int genSize, int trSize)
{
	for (int c = 0; c < genSize; ++c){

		Image image(width, height);

		for (int i = 0; i < trSize; i++)
		{
			Triangle tr = rnd.getRandomTriangle(width, height);
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
	SDLWrapper sdl(1200, 600);
	if (!sdl.initSDL())
	{
		return -1;
	}

	int MaxTriangles = 40;
	int MaxGenSize = 30;
	Random rnd;

	SDL_Surface* original = sdl.drawImageFromPath("../Images/tr.png");

	GA ga(MaxTriangles, original);

	firstGeneration(original->w, original->h, rnd, MaxGenSize, MaxTriangles);

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
			m = rnd.getIndex(1, MaxGenSize - 1);
			f = rnd.getIndex(0, m - 1);
			Vector2 from(0,0), to(0,0);
			newGen.push_back(std::move(ga.cross(generation[m], generation[f])));
			//sdl.drawLine(from, to, Color(255, 0, 0, 255));
		}

		//add the best ones
		for (int i = 0; i < MaxGenSize/10; i++)
		{
			newGen.push_back(std::move(generation[i]));
		}

		//mutate
		int k;
		for (int i = 0; i < MaxGenSize; i++)
		{
			k = rnd.getIndex(0, 100);
			if (k < 5) ga.mutate(newGen[i]);
		}

		generation = std::move(newGen);

		sdl.checkForEvent();

	}

	return 0;
}


