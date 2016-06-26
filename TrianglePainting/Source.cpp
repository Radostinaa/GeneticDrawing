#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"
#include "Image.h"
#include "GA.h"
#include <thread>

class Utils
{
public:
	Utils(int genSize, int trSize) : MaxTriangles(trSize), GenSize(genSize)
	{	}

	std::vector<Image> generation;
	int MaxTriangles;
	int GenSize;

	Random rnd;
};

void firstGeneration(int width, int height, Random& rnd, int genSize, int trSize, std::vector<Image>& generation)
{
	for (int c = 0; c < genSize; ++c) {

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

void sotrGeneration(std::vector<Image>& generation)
{
	std::sort(generation.begin(), generation.end());
}

void bla(int from, int size, std::vector<Image>& newGen, GA& ga, Utils& ut)
{
	//cross
	int m, f;
	for (int i = from; i < from + size; i++)
	{
		m = ut.rnd.getIndex(1, ut.GenSize - 1);
		f = ut.rnd.getIndex(0, m - 1);
		newGen.push_back(std::move(ga.cross(ut.generation[m], ut.generation[f])));
	}

	//mutate
	int k;
	for (int i = 0; i < size; i++)
	{
		k = ut.rnd.getIndex(0, 100);
		if (k < 5) ga.mutate(newGen[i]);
	}
}

int main(int argc, char *argv[])
{
	SDLWrapper sdl(1200, 600);
	if (!sdl.initSDL())
	{
		return -1;
	}

	Random rnd;
	Utils ut(40, 40);

	SDL_Surface* original = sdl.drawImageFromPath("../Images/cookie_monster.png");

	GA ga(ut.MaxTriangles, original);

	const int hardwareThreads = static_cast<int>(std::thread::hardware_concurrency());
	int thCount = std::min(64, hardwareThreads != 0 ? hardwareThreads : 1);
	std::vector<std::thread> threads(thCount);
	auto sz = ut.GenSize / thCount;

	firstGeneration(original->w, original->h, rnd, ut.GenSize, ut.MaxTriangles, ut.generation);

	std::vector<std::vector<Image>> newGens(thCount);
	std::vector<Image> newGen;
	int cg = 0;
	while (!sdl.Quit())
	{
		sdl.clear();
		sdl.drawImage(original, true);

		for (auto & subGen : newGens)
		{
			subGen.clear();
		}

		//get fitness
		for (auto &img : ut.generation) 
		{
			ga.fitness(img);
		}
		sotrGeneration(ut.generation);

		//draw best image
		sdl.drawImage(ut.generation[0].surface, false);

		for (int i = 0; i < thCount; i++)
		{
			threads[i] = std::thread(bla, i*sz, sz, std::ref(newGens[i]), std::ref(ga), std::ref(ut));
		}

		for (int i = 0; i < thCount; i++)
		{
			threads[i].join();
		}

		newGen.clear();
		for (auto & subGen : newGens) {
			for (auto & img : subGen) {
				newGen.push_back(std::move(img));
			}
		}

		//add the best ones
		for (int i = 0; i < ut.GenSize / 10; i++)
		{
			newGen[i] = std::move(ut.generation[i]);
		}
		ut.generation.swap(newGen);

		sdl.checkForEvent();

		printf("Generation %d\n", cg++);
	}

	SDL_FreeSurface(original);
	return 0;
}


