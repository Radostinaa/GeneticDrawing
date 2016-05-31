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
	rect.w = height/2 - 20;
	rect.h = width/2;
	rect.x = 0;
	rect.y = 0;

	SDL_RenderCopy(render, texture, NULL, &rect);
}


void SDLWrapper::drawLine(const Vector2& from, const Vector2& to, Color& color)
{
	SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 0);
	SDL_RenderDrawLine(render, from.x, from.y, to.x, to.y);
}

void sortVerticesAscendingByY(Vector2& v1, Vector2& v2, Vector2& v3)
{
	Vector2 temp();	if (v1.y < v2.y) 
	{
		if (v2.y < v3.y)
		{
			temp
		}
	}
}

void SDLWrapper::drawTirangle(Vector2& v1, Vector2& v2, Vector2& v3, Color& color)
{
	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	sortVerticesAscendingByY(v1, v2, v3);

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (v2.y == v3.y)
	{
		fillBottomFlatTriangle(v1, v2, v3, color);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		fillTopFlatTriangle(v1, v2, v3, color);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vector2 v4 = Vector2(
			(int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
		fillBottomFlatTriangle( v1, v2, v4, color);
		fillTopFlatTriangle(v2, v4, v3, color);
	}
}

void fillBottomFlatTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color &color)
{
	float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
	float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

	float curx1 = v1.x;
	float curx2 = v1.x;

	for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
	{
		drawLine(Vector2( (int)curx1, scanlineY), Vector2((int)curx2, scanlineY), color);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void fillTopFlatTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color &color)
{
	float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
	float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

	float curx1 = v3.x;
	float curx2 = v3.x;

	for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
	{
		drawLine(Vector2((int)curx1, scanlineY), Vector2((int)curx2, scanlineY), color);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}


