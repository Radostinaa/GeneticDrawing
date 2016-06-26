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

void DrawPixel(SDL_Surface* target, Sint16 x, Sint16 y, Uint32 color) {
	Uint32 * pixels = (Uint32*)target->pixels;
	pixels[y * target->w + x] = color;
}

Uint32 ReadPixel(SDL_Surface* source, Sint16 x, Sint16 y) {
	Uint32 * pixels = (Uint32 *)source->pixels;
	return pixels[y * source->w + x];
}

SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
	if (!Surface || !Width || !Height)
		return 0;

	SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
		Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

	double _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(Surface->w)),
		   _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(Surface->h));

	for (Sint32 y = 0; y < Surface->h; y++) {
		for (Sint32 x = 0; x < Surface->w; x++) {
			for (Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y) {
				for (Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x) {
					DrawPixel(_ret, static_cast<Sint32>(_stretch_factor_x * x) + o_x,
						static_cast<Sint32>(_stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));
				}
			}
		}
	}

	return _ret;
}

SDL_Surface* SDLWrapper::drawImageFromPath(char* path)
{
	SDL_Surface *image;
	image = IMG_Load(path);
	if (!image)
	{
		printf(" nananaan %s\n", IMG_GetError());
	}

	auto scaled = ScaleSurface(image, 250, 250);
	SDL_FreeSurface(image);
	image = scaled;


	SDL_Texture * texture = SDL_CreateTextureFromSurface(render, image);

	SDL_Rect rect;
	rect.w = image->w; // width / 2;
	rect.h = image->h; // width / 2;
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
	rect.w = image->w; // width / 2;
	rect.h = image->h; //width / 2;
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