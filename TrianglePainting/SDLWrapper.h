#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"
#include <vector>
#include "Image.h"

class SDLWrapper
{
private:
	int width;
	int height;
	SDL_Window* window;
	SDL_Renderer* render;
	bool quit;

public:
	SDLWrapper(const int width, const int height);
	~SDLWrapper();
	bool initSDL();

	int getWidth(){ return width; }
	int getHeight(){ return height; }

	bool Quit(){ return quit; };
	void checkForEvent();

	void setWinTitle(const char * title) { SDL_SetWindowTitle(window, title); }

	SDL_Surface* drawImageFromPath(char* path);
	void drawImage(Image img);
	/*void drawTirangle(Triangle t);*/
private:
	void update() { SDL_RenderPresent(render); }
	//void drawLine(const Vector2& from, const Vector2& to, const Color& color);

	//void fillBottomFlatTriangle(const Triangle& t);
	//void fillTopFlatTriangle(const Triangle& t);

	//void tr(Triangle& t);
};