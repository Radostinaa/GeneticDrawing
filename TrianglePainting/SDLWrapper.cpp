#include "SDLWrapper.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <algorithm> 

SDLWrapper::SDLWrapper(const int width, const int height) : width(width), height(height),
	quit(false) ,window(NULL), render(NULL){}


SDLWrapper::~SDLWrapper()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	SDL_Quit();
} 

void SDLWrapper::checkForEvent()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
	}
}

bool SDLWrapper::initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	window = SDL_CreateWindow("nana", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_SHOWN);
	if (!window)
	{
		SDL_Quit();
		return false;
	}

	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!render)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	return true;
}

void SDLWrapper::drawImage(char* path)
{
	SDL_Surface *image;
	image = IMG_Load(path);
	if (!image)
	{
		printf(" nananaan %s\n", IMG_GetError());
	}

	SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);

	SDL_Rect rect;
	rect.w = width / 2;
	rect.h = width / 2;
	rect.x = 0;
	rect.y = 0;

	SDL_RenderCopy(render, texture, NULL, &rect);
}


void SDLWrapper::drawLine(const Vector2& from, const Vector2& to, const Color& color)
{
	SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
	SDL_RenderDrawLine(render, from.x, from.y, to.x, to.y);
}

void swapIf(Vector2& v1, Vector2& v2)
{
	Vector2 temp(0, 0);
	if (v2 <= v1)
	{
		temp = v1;
		v1 = v2;
		v2 = temp;
	}
}

void sortVerticesAscendingByY(Vector2& v1, Vector2& v2, Vector2& v3)
{
	swapIf(v1, v2);
	swapIf(v2, v3);
	swapIf(v1, v3);
}

void SDLWrapper::drawTirangle(Triangle t, const Color& color)
{
	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(t.v1, t.v2, t.v3);

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (t.v2.y == t.v3.y)
	{
		fillBottomFlatTriangle({ t.v1, t.v2, t.v3 }, color);
	}
	/* check for trivial case of top-flat triangle */
	else if (t.v1.y == t.v2.y)
	{
		fillTopFlatTriangle({ t.v1, t.v2, t.v3 }, color);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2(
			(int)(t.v1.x + ((float)(t.v2.y - t.v1.y) / (float)(t.v3.y - t.v1.y)) * (t.v3.x - t.v1.x)), t.v2.y);
		fillBottomFlatTriangle({ t.v1, t.v2, v4 }, color);
		fillTopFlatTriangle({ t.v2, v4, t.v3 }, color);
	}
}

void SDLWrapper::fillBottomFlatTriangle(const Triangle& t, const Color & color)
{
	float invslope1 = (t.v2.x - t.v1.x) / (t.v2.y - t.v1.y);
	float invslope2 = (t.v3.x - t.v1.x) / (t.v3.y - t.v1.y);

	float curx1 = t.v1.x;
	float curx2 = t.v1.x;

	for (int scanlineY = t.v1.y; scanlineY <= t.v2.y; scanlineY++)
	{
		drawLine(Vector2((int)curx1, scanlineY), Vector2((int)curx2, scanlineY), color);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}
void SDLWrapper::fillTopFlatTriangle(const Triangle& t, const Color & color)
{
	float invslope1 = (t.v3.x - t.v1.x) / (t.v3.y - t.v1.y);
	float invslope2 = (t.v3.x - t.v2.x) / (t.v3.y - t.v2.y);

	float curx1 = t.v3.x;
	float curx2 = t.v3.x;

	for (int scanlineY = t.v3.y; scanlineY > t.v1.y; scanlineY--)
	{
		drawLine(Vector2((int)curx1, scanlineY), Vector2((int)curx2, scanlineY), color);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}


