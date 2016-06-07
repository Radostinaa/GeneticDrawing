#pragma once
#include <SDL.h>
#include "Vector.h"
#include <vector>

class Image
{
public:
	SDL_Surface* surface;
	Uint32 * pixels;
	std::vector<Triangle> triangles;
	double fitness;

	Image(int _width, int _height);
	SDL_Surface* getImage();

private:

	void drawLine(const Vector2& from, const Vector2& to, const Color& color);

	void drawTirangle(Triangle t);

	void fillBottomFlatTriangle(const Triangle& t);

	void fillTopFlatTriangle(const Triangle& t);

	Uint32 calcPixel(Uint32* pixels, int x, int y, Color color);
};
