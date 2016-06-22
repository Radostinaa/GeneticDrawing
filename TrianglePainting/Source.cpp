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
	Utils(int genSize, int trSize): MaxTriangles(trSize), GenSize(genSize)
	{	}

	std::vector<Image> generation;
	int MaxTriangles;
	int GenSize;

	Random rnd;
};
std::vector<Image> generation;
void firstGeneration(int width, int height, Random& rnd, int genSize, int trSize, std::vector<Image>& generation)
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

void sotrGeneration(std::vector<Image>& generation)
{
	std::sort(generation.begin(), generation.end());
}

void bla(int from, int size, std::vector<Image>& newGen, GA& ga, Utils& ut)
{
	//cross
	int m, f;
	for (int i = from; i < size; i++)
	{
		m = ut.rnd.getIndex(from + 1, size - 1);
		f = ut.rnd.getIndex(from, m - 1);
		Vector2 from(0, 0), to(0, 0);
		newGen.push_back(std::move(ga.cross(ut.generation[m], ut.generation[f])));
	}

	//mutate
	int k;
	for (int i = from; i < size; i++)
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
	Utils ut(30, 40);

	SDL_Surface* original = sdl.drawImageFromPath("../Images/tr.png");

	GA ga(ut.MaxTriangles, original);

	const int hardwareThreads = static_cast<int>(std::thread::hardware_concurrency());
	int thCount = std::min(64, hardwareThreads != 0 ? hardwareThreads : 1);
	std::vector<std::thread> threads(thCount);

	firstGeneration(original->w, original->h, rnd, ut.GenSize, ut.MaxTriangles, ut.generation);

	while (!sdl.Quit())
	{
		sdl.clear();
		sdl.drawImage(original, true);

		//get fitness
		for (auto &img : generation)
		{
			ga.fitness(img);
		}

		sotrGeneration(ut.generation);

		//draw best image
		sdl.drawImage(generation[0].surface, false);

		std::vector<Image> newGen;
		newGen.reserve(ut.GenSize);

		for (int i = 0; i < thCount - 1; i++)
		{
			threads[i] = std::thread(bla, ut.GenSize / thCount, i*(ut.GenSize / thCount), newGen, ga, ut);
		}

		for (int i = 0; i < thCount - 1; i++)
		{
			threads[i].join();
		}

		//add the best ones
		for (int i = 0; i < ut.GenSize / 10; i++)
		{
			newGen.push_back(std::move(generation[i]));
		}
		generation = std::move(newGen);

		sdl.checkForEvent();

	}

	return 0;
}


