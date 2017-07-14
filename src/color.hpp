#pragma once

class Color3 {
public:
	Color3(const unsigned char r = 0,
	       const unsigned char g = 0,
	       const unsigned char b = 0);

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Color4 {
public:
	static const Color4 Black;
	static const Color4 White;
	static const Color4 Transparent;

	Color4(const unsigned char r = 0,
	       const unsigned char g = 0,
	       const unsigned char b = 0,
	       const unsigned char a = 255);

	explicit operator Color3() const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};
