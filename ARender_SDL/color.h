#pragma once


struct Color {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char bgra[4];
		unsigned int val;
	};
	int bytespp;

	Color() : val(0), bytespp(1) { }

	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {}

	Color(int v, int bpp) : val(v), bytespp(bpp) { }

	Color(const Color& c) : val(c.val), bytespp(c.bytespp) { }

	Color(const unsigned char* p, int bpp) : val(0), bytespp(bpp) {
		for (int i = 0; i < bpp; i++) {
			bgra[i] = p[i];
		}
	}

	Color& operator =(const Color& c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};