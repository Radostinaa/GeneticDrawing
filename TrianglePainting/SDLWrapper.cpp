#include "SDLWrapper.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <algorithm> 
#include <vector>

SDLWrapper::SDLWrapper(const int width, const int height) : width(width), height(height),
quit(false), window(NULL), render(NULL){}


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

	screen = SDL_GetWindowSurface(window);
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

SDL_Surface* SDLWrapper::drawImageFromPath(char* path)
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
	update();
	return image;
}

void SDLWrapper::drawImage(SDL_Surface *image, bool original)
{
	SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);


	SDL_Rect rect;
	rect.w = width / 2;
	rect.h = width / 2;
	rect.x = original ? 0 : width / 2;;
	rect.y = 0;

	SDL_RenderCopy(render, texture, NULL, &rect);
	update();
}

void SDLWrapper::drawLine(const Vector2& from, const Vector2& to, const Color& color)
{
	SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
	SDL_RenderDrawLine(render, from.x + width / 2, from.y, to.x + width / 2, to.y);
	update();
}