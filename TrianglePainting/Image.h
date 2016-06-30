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

    inline bool inBounds(int x, int y) {
        return x > 0 && y > 0 && x < surface->w && y < surface->h;
    }


    Image() : surface(nullptr), pixels(nullptr), fitness(0) {}


	Image(int _width, int _height);
	~Image();
	SDL_Surface* generatePixels();

	Image(Image&& i)
	{
		this->triangles = i.triangles;
		this->pixels = i.pixels;
		this->fitness = i.fitness;
		this->surface = i.surface;

		i.pixels = nullptr;
		i.surface = nullptr;
	}

	Image & operator=(Image && o)
	{
		std::swap(this->triangles, o.triangles);
		std::swap(this->pixels, o.pixels);
		std::swap(this->fitness, o.fitness);
		std::swap(this->surface, o.surface);

		return *this;
	}

	Image(const Image& img) = delete;
	Image& operator=(const Image& other) = delete;

private:

	void drawLine(const Vector2& from, const Vector2& to, const Color& color);

	void drawTirangle(Triangle t);

	void fillBottomFlatTriangle(const Triangle& t);

	void fillTopFlatTriangle(const Triangle& t);

	Uint32 calcPixel(Uint32* pixels, int x, int y, Color color);
};
