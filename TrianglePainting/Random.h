#pragma once
#include <random>
#include "Vector.h"

class Random
{
private:
	std::random_device device;
	std::mt19937_64 generator;

	inline Color getRandomColor();
	inline Vector2 getRandomCoordinate(int width, int height);

public:
	Random() : generator(device()){}
	inline Triangle getRandomTriangle(int width, int height);
};