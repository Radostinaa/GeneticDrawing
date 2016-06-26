#include"Image.h"
#include"Vector.h"

Image::Image(int _width, int _height)
{
	this->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, _width, _height, 32,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	this->pixels = (Uint32 *)surface->pixels;

	for (int c = 0; c < surface->w * surface->h; ++c)
		pixels[c] = Color(0, 0, 0, 255);
}

Image::~Image()
{
	SDL_FreeSurface(this->surface);
	this->surface = nullptr;
	pixels = nullptr;
}

Uint32 Image::calcPixel(Uint32* pixels, int x, int y, Color color)
{
	Uint32 * dest = &pixels[(y * (surface->w)) + x];
	*dest = *((Color*)dest) + color;
	return *dest ;
}

//bresenham line
void Image::drawLine(const Vector2& from, const Vector2& to, const Color& color) {
	
	Vector2 f(from);

	float dx = abs(to.x - f.x), sx = f.x < to.x ? 1 : -1;
	float dy = abs(to.y - f.y), sy = f.y < to.y ? 1 : -1;
	float err = (dx>dy ? dx : -dy) / 2;

	while (true) {
		pixels[((int)f.y * (surface->w)) + (int)f.x] = calcPixel(pixels, f.x, f.y, color);
		if (f.x == to.x && f.y == to.y) break;
		float e2 = err;
		if (e2 > -dx) {
			err -= dy;
			f.x += sx;
		}
		if (e2 < dy) {
			err += dx;
			f.y += sy;
		}
	}
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
	swapIf(v1, v2);
}

SDL_Surface* Image::generatePixels()
{
	for (auto tr : triangles)
	{
		drawTirangle(tr);
	}

	return this->surface;
}

void Image::drawTirangle(Triangle t)
{
	sortVerticesAscendingByY(t.v1, t.v2, t.v3);

	if (t.v2.y == t.v3.y)
	{
		fillBottomFlatTriangle({ t.v1, t.v2, t.v3, t.color });
	}
	else if (t.v1.y == t.v2.y)
	{
		fillTopFlatTriangle({ t.v1, t.v2, t.v3, t.color });
	}
	else
	{
		Vector2 v4 = Vector2(
			(int)(t.v1.x + ((float)(t.v2.y - t.v1.y) / (float)(t.v3.y - t.v1.y)) * (t.v3.x - t.v1.x)), t.v2.y);
		fillBottomFlatTriangle({ t.v1, t.v2, v4, t.color });
		fillTopFlatTriangle({ t.v2, v4, t.v3, t.color });
	}
}

void Image::fillBottomFlatTriangle(const Triangle& t)
{
	float invslope1 = (t.v2.x - t.v1.x) / (t.v2.y - t.v1.y);
	float invslope2 = (t.v3.x - t.v1.x) / (t.v3.y - t.v1.y);

	float curx1 = t.v1.x;
	float curx2 = t.v1.x;

	for (int scanlineY = t.v1.y; scanlineY <= t.v2.y; scanlineY++)
	{
		drawLine(Vector2((int)curx1, scanlineY), Vector2((int)curx2, scanlineY), t.color);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}
void Image::fillTopFlatTriangle(const Triangle& t)
{
	float invslope1 = (t.v3.x - t.v1.x) / (t.v3.y - t.v1.y);
	float invslope2 = (t.v3.x - t.v2.x) / (t.v3.y - t.v2.y);

	float curx1 = t.v3.x;
	float curx2 = t.v3.x;

	for (int scanlineY = t.v3.y; scanlineY > t.v1.y; scanlineY--)
	{
		drawLine(Vector2((int)curx1, scanlineY), Vector2((int)curx2, scanlineY), t.color);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}