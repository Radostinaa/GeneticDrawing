#pragma once
#include "Image.h"
#include "Vector.h"
#include <SDL.h>
#include "Random.h"

class GA
{
public:
	GA(const int _trianglesCount, const SDL_Surface* _original) : trianglesCount(_trianglesCount)
	{
		this->originalPixels = (Uint32 *)_original->pixels;
	}

	void fitness(Image& image);
	Image cross(const Image& mother, const Image& fother);
	void mutate(Image& image);

private:
	int trianglesCount;
	Uint32 * originalPixels;
	const SDL_Surface* original;
	Random rnd;
	bool isOnRightTriangle(const Triangle& tr, const Vector2& p1, const Vector2& p2);
	bool isOnRightPint(const Vector2& p1, const Vector2& p2, const Vector2& p3);
};