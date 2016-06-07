#pragma once
#include "Image.h"
#include "Vector.h"
#include<SDL.h>

class GA
{
public:
	GA(const int _genSize, const SDL_Surface* _original) : genSize(_genSize)
	{
		this->originaPixels = (Uint32 *)_original->pixels;
	}

	double fitness(const Image& image);
	void cross();
	void mutate();

private:
	int genSize;
	Uint32 * originaPixels;
	const SDL_Surface* original;
};