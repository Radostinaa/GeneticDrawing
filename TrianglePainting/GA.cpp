#include "GA.h"

void GA::fitness(Image& image)
{
	double error = 0;

	for (int c = 0; c < image.surface->w * image.surface->h; ++c)
	{
		Color c1 = *(Color*)&image.pixels[c];
		Color c2 = *(Color*)&originalPixels[c];

		error += Colorfitness(c1, c2);
	}

	image.fitness = error;
}

bool isNotUsed(std::vector<Triangle>& used, const Triangle& tr)
{
	for (auto &t : used)
	{
		if (t == tr) return false;
	}
	return true;
}


Image GA::cross(const Image& mother, const Image& father, std::vector<Triangle>& used)
{
	Image child(mother.surface->w, mother.surface->h);
	child.triangles.reserve(trianglesCount);

	// get random line trought the image 
	Vector2 p1 = rnd.getRandomCoordinate(mother.surface->w, mother.surface->h);
	Vector2 p2 = rnd.getRandomCoordinate(mother.surface->w, mother.surface->h);

	p2 = p1 == p2 ? Vector2(p1.y, p1.x) : p2;

	int count = 0; // count of triangles added int child

	for (auto & tr : father.triangles)
	{
		if (count >= trianglesCount) break;
		if (isOnRightTriangle(tr, p1, p2))
		{
			child.triangles.push_back(tr);
			used.push_back(tr);
			count++;
		}
	}

	for (auto & tr : mother.triangles)
	{
		if (count >= trianglesCount) break;
		if (!isOnRightTriangle(tr, p1, p2))
		{
			child.triangles.push_back(tr);
			used.push_back(tr);
			count++;
		}
	}

	if (count < trianglesCount)
	{
		for (int c = 0; c < trianglesCount; ++c)
		{
			if (count < trianglesCount){
				if (isNotUsed(used, father.triangles[c]))
				{
					child.triangles.push_back(father.triangles[c]);
					count++;
				}
			}
		}
	}

	if (child.triangles.size() != trianglesCount) __debugbreak();
	child.generatePixels();

	return child;
}

bool GA::isOnRightTriangle(const Triangle& tr, const Vector2& p1, const Vector2& p2)
{
	int right = 0;
	right += isOnRightPint(p1, p2, tr.v1) ? 1 : 0;
	right += isOnRightPint(p1, p2, tr.v2) ? 1 : 0;
	right += isOnRightPint(p1, p2, tr.v3) ? 1 : 0;

	return right >= 2 ? true : false;
}

bool GA::isOnRightPint(const Vector2& p1, const Vector2& p2, const Vector2& p3)
{
	float value = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);

	if (value >= 0) return false;
	if (value < 0) return true;
}


void GA::mutate(Image& image)
{
	// replace count random triangles from the image with new random ones
	int count = rnd.getIndex(1, image.triangles.size() - 1);
	int ix;
	for (int i = 0; i < count; i++)
	{
		ix = rnd.getIndex(0, image.triangles.size() - 1);
		image.triangles[ix] = std::move(rnd.getRandomTriangle(image.surface->w, image.surface->h));
	}

	// swaitch count random triangles from thes painting
	int f, s;
	Triangle temp;
	count = rnd.getIndex(1, image.triangles.size() / 2);
	for (int i = 0; i < count; i++)
	{
		f = rnd.getIndex(1, image.triangles.size() - 1);
		s = rnd.getIndex(1, image.triangles.size() - 1);

		temp = image.triangles[f];
		image.triangles[f] = image.triangles[s];
		image.triangles[s] = temp;
	}

}