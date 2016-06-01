#pragma once
#include<SDL.h>
class Vector2
{
public:
	float x, y;

	Vector2(const float _x, const float _y) : x(_x), y(_y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}

	inline void operator = (const Vector2& v)
	{
		if (this != &v)
		{
			this->x = v.x;
			this->y = v.y;
		}
	}
};


inline Vector2 operator + (const Vector2& a, const Vector2& b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}

inline Vector2 operator - (const Vector2& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

inline bool operator <= (const Vector2& a, const Vector2& b)
{
	return a.y <= b.y;
}


inline bool operator >= (const Vector2& a, const Vector2& b)
{
	return a.y >= b.y;
}

/// cross product
inline Vector2 operator ^ (const Vector2& a, const Vector2& b)
{
	/*return Vector2(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
		);*/
}

class Color
{
public:
	Uint8 r, g, b, a;

	Color(const Uint8 _r, const Uint8 _g, const Uint8 _b, const Uint8 _a) : r(_r), g(_g), b(_b), a(_a) {}

};

inline Color operator + (const Color& a, const Color& b)
{
	return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

inline Color operator - (const Color& a, const Color& b)
{
	return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}


struct Triangle
{
	Vector2 v1, v2, v3;
	Color color;
	Triangle(Vector2 _v1, Vector2 _v2, Vector2 _v3, Color _color) : v1(_v1), v2(_v2), v3(_v3), color(_color) {}

	Triangle(const Triangle& t) :v1(t.v1), v2(t.v2), v3(t.v3), color(t.color) {}

	inline void operator = (const Triangle& t)
	{
		if (this != &t)
		{
			this->v1 = t.v1;
			this->v2 = t.v2;
			this->v3 = t.v3;
			this->color = t.color;
		}
	}
};
