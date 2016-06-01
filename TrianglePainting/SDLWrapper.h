#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"

class SDLWrapper
{
private:
	int width;
	int height;
	SDL_Window * window;
	SDL_Renderer * render;
	bool quit;

public:
	SDLWrapper(const int width, const int height);
	~SDLWrapper();
	bool initSDL();

	int getWidth(){ return width; }
	int getHeight(){ return height; }

	bool Quit(){ return quit; };
	void checkForEvent();

	void update() { SDL_RenderPresent(render); }
	void setWinTitle(const char * title) { SDL_SetWindowTitle(window, title); }

	void drawImage(char* path);

	void drawTirangle(Triangle t);
	void drawLine(const Vector2& from, const Vector2& to, const Color& color);

private:

	void fillTopFlatTriangle(const Triangle& t);
	void fillBottomFlatTriangle(const Triangle& t);
};