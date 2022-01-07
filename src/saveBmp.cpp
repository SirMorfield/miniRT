// This is very very bad
#include "main.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <fstream>

#define HEADER_SIZE 54

static size_t bmp_size(size_t x, size_t y) {
	if ((x * 3) % 4 != 0)
		x = x + 4 - ((x * 3) % 4);
	return (HEADER_SIZE + (3 * y * x));
}

/*
** HEADER
** Position | Hex	| Description
** 00		| 00	| TYPE OF BMP FILE
** 02		| 02	| TOTAL SIZE OF FILE
** 06-08	| 06-08	| RESERVED TO SIGN
** 10		| 0A	| HEADER SIZE
** 14		| 0E	| HEADER INFOS SIZE
** 18		| 12	| WIDTH OF IMG (PX)
** 22		| 16	| HEIGHT OF IMG (PX)
** 26		| 1A	| NUMBER OF PLANS
** 28		| 1C	| BITS PER PIXELS (1 - 4 - 8 - 24)
** 30		| 1E	| COMPRESSION TYPE
** 34		| 22	| SIZE OF IMG
** 38		| 26	| RES X PX PER METER
** 42		| 2A	| RES Y PX PER METER
** 46		| 2E	| NB OF USED COLOR (0 = MAX)
** 50		| 32	| NB OF INDEX OF COLOR (0 = ALL)
*/

static void header(uint8_t* buf, uint32_t x, uint32_t y) {
	*((uint8_t*)&buf[0]) = (uint8_t)0x42;
	*((uint8_t*)&buf[1]) = (uint8_t)0x4D;
	*((uint32_t*)&buf[2]) = (uint32_t)bmp_size(x, y);
	*((uint32_t*)&buf[10]) = (uint32_t)HEADER_SIZE;
	*((uint32_t*)&buf[14]) = (uint32_t)0x28;
	*((uint32_t*)&buf[18]) = (uint32_t)x;
	*((uint32_t*)&buf[22]) = (uint32_t)y;
	*((uint16_t*)&buf[26]) = (uint16_t)0x01;
	*((uint16_t*)&buf[28]) = (uint16_t)0x18;
}

bool save_bmp(size_t xSize, size_t ySize, const std::vector<Rgb>& frame, const std::string path) {

	std::ofstream pFile(path, std::ios_base::binary);
	if (!pFile.is_open())
		return false;
	uint8_t* buf = (uint8_t*)calloc(bmp_size(xSize, ySize), 1);
	header(buf, xSize, ySize);
	size_t buf_i = HEADER_SIZE;
	size_t y = ySize;
	while (y > 0) {
		y--;
		size_t x = 0;
		while (x < xSize) {
			const Rgb pixel = frame[y * xSize + x];
			buf[buf_i + 0] = pixel.r;
			buf[buf_i + 1] = pixel.g;
			buf[buf_i + 2] = pixel.b;
			buf_i += 3;
			x++;
		}
		if ((xSize * 3) % 4 != 0)
			buf_i += 4 - (xSize * 3) % 4;
	}

	pFile.write((const char*)buf, bmp_size(xSize, ySize));
	pFile.close();
	return true;
}
