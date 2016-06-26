#pragma once
#include <random>
#include "Vector.h"

static std::random_device device;

class Random
{
private:
	std::mt19937_64 generator;

	Color getRandomColor();

public:
	Random() : generator(device()){}
	Triangle getRandomTriangle(int width, int height);
	int getIndex(int from, int to);
	Vector2 getRandomCoordinate(int width, int height);
};