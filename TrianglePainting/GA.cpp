#include "GA.h"

double GA::fitness(const Image& image)
{
	double error = 0;

	for (int c = 0; c < image.surface->w * image.surface->h; ++c)
	{
		Color c1 = *(Color*)image.pixels[c];
		Color c2 = *(Color*)originaPixels[c];;
		error += Colorfitness(c1, c2);
	}

	return error;
}