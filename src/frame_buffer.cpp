// This is very very bad
#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>

#define HEADER_SIZE 54

Resolution::Resolution(size_t x, size_t y, size_t AA_level)
	: _width(x),
	  _height(y),
	  _AA_level(AA_level) {
	assert(_AA_level == 1 || is_power_of_2(_AA_level));
}

Resolution::Resolution(Resolution::Standard_size standard_size, size_t AA_level) {
	switch (standard_size) {
	case Resolution::Standard_size::R_8K:
		new (this) Resolution(7680, 4320, AA_level);
		break;
	case Resolution::Standard_size::R_5K:
		new (this) Resolution(5120, 2880, AA_level);
		break;
	case Resolution::Standard_size::R_4K:
		new (this) Resolution(3840, 2160, AA_level);
		break;
	case Resolution::Standard_size::R_1080p:
		new (this) Resolution(1920, 1080, AA_level);
		break;
	case Resolution::Standard_size::R_720p:
		new (this) Resolution(1280, 720, AA_level);
		break;
	case Resolution::Standard_size::R_480p:
		new (this) Resolution(720, 480, AA_level);
		break;
	case Resolution::Standard_size::R_360p:
		new (this) Resolution(640, 360, AA_level);
		break;
	case Resolution::Standard_size::R_240p:
		new (this) Resolution(426, 240, AA_level);
		break;
	case Resolution::Standard_size::R_144p:
		new (this) Resolution(256, 144, AA_level);
		break;
	}
}
std::ostream& operator<<(std::ostream& os, const Resolution& res) {
	os << res.width() << "X" << res.height() << " " << res.AA_level() << "AA";
	return os;
}

Random_counter::Random_counter(size_t max)
	: _i(0),
	  _offset(0),
	  _change(std::max((size_t)3, closest_prime<size_t>(max / 10))),
	  _done(0),
	  _max(max) {}
void Random_counter::reset() {
	_i = 0;
	_offset = 0;
	_done = 0;
}

std::optional<size_t> Random_counter::next() {
	if (_done >= _max)
		return {};

	_i += _change;
	if (_i > _max)
		_i = ++_offset;
	_done++;
	return _i;
}

size_t Random_counter::get() const { return _i; }

Frame_buffer::Frame_buffer(size_t x_size, size_t y_size, bool log_progress)
	: _x_size(x_size),
	  _y_size(y_size),
	  _log_progress(log_progress),
	  _i(x_size * y_size),
	  _pix_done(0) {

	_frame.resize(x_size * y_size);
	for (size_t i = 0; i < _frame.size(); i++)
		_frame.at(i) = Rgb(255, 0, 0);
}

// // TODO: get random pixel instead of sequencial
std::optional<Point2<size_t>> Frame_buffer::get_pixel() {
	_mutex.lock();
	std::optional<Point2<size_t>> pixel;

	if (_pix_done >= _frame.size())
		goto end;

	if (!_i.next().has_value())
		goto end;

	pixel = Point2<size_t>((_i.get() - 1) % _x_size, (_i.get() - 1) / _x_size);
	_pix_done++;

	if (!_log_progress)
		goto end;

	if (_pix_done == 1)
		_progress.start();
	_progress.print((_pix_done / (float)_frame.size()) * 100);

end:
	_mutex.unlock();
	return pixel;
}

void Frame_buffer::set_pixel(const Rgb& color, size_t x, size_t y) {
	_frame[y * _x_size + x] = color;
}

void Frame_buffer::save_to_BMP() const {
	assert(_pix_done == _frame.size());
	save_bmp(_x_size, _y_size, _frame, "scene.bmp");
}

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
	free(buf);
	pFile.close();
	return true;
}
