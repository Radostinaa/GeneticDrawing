#include "Random.h"

inline Vector2 Random::getRandomCoordinate(int width, int height)
{
	std::uniform_real_distribution<float> distX(0, width);
	float x = distX(generator);

	std::uniform_real_distribution<float> distY(0, height);
	float y = distX(generator);

	return Vector2(x, y);
}


Triangle Random::getRandomTriangle(int width, int height)
{
	Vector2 v1 = getRandomCoordinate(width, height);
	Vector2 v2 = getRandomCoordinate(width, height);
	Vector2 v3 = getRandomCoordinate(width, height);
	Color c = getRandomColor();
	return Triangle(v1, v2, v3, c);
}

inline Color Random::getRandomColor()
{
	std::uniform_int_distribution<int> distA(25, 153);
	Uint8 a = distA(generator);

	std::uniform_int_distribution<int> distRGB(0, 255);
	Uint8 r = distRGB(generator);

	Uint8 g = distRGB(generator);

	Uint8 b = distRGB(generator);


	return Color(r, g, b, a);
}