#include "color.hpp"

Color3::Color3(const unsigned char r,
               const unsigned char g,
               const unsigned char b)
	: r(r),
	  g(g),
	  b(b)
{
}

const Color4 Color4::Black(0, 0, 0);
const Color4 Color4::White(255, 255, 255);
const Color4 Color4::Transparent(0, 0, 0, 255);

Color4::Color4(const unsigned char r,
               const unsigned char g,
               const unsigned char b,
               const unsigned char a)
	: r(r),
	  g(g),
	  b(b),
	  a(a)
{
}

Color4::operator Color3() const
{
	return {this->r, this->g, this->b};
}
