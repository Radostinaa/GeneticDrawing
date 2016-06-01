#include "SDLWrapper.h"
#include <stdio.h>
#include<vector>
#include "Random.h"

int MaxTriangles = 50;

int main(int argc, char *argv[])
{
	SDLWrapper sdl(900, 500);
	if (!sdl.initSDL())
	{
		return -1;
	}

	sdl.drawImage("../Images/cookie_monster.png");
	sdl.update();

	//sdl.drawTirangle({ { 500,10 },{ 200, 200 },{ 100,200 } }, { 255, 0, 0 , 10});

	std::vector<Triangle> triangles;
	Random rnd;
	for (int i = 0; i < MaxTriangles; i++)
	{
		Triangle tr = rnd.getRandomTriangle(500, 500);
		triangles.push_back(tr);
	}

	for (auto tr : triangles)
	{
		sdl.drawTirangle(tr);
	}

	sdl.update();


	while (1) { SDL_Delay(1); }
	return 0;
}

void GenerateFirstGeneration()
{

}