#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"

struct Triangle
{
	Vector2 v1, v2, v3;
	Triangle(Vector2 _v1, Vector2 _v2, Vector2 _v3) : v1(_v1), v2(_v2), v3(_v3) {}
};


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

	void drawTirangle(Triangle t, const Color& color);
	void drawLine(const Vector2& from, const Vector2& to, const Color& color);

private:

	void fillTopFlatTriangle(const Triangle& t, const Color &color);
	void fillBottomFlatTriangle(const Triangle& t, const Color &color);
};