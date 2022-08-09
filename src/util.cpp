#include "util.hpp"
#include "vector.hpp"
#include <cmath>

std::vector<std::string> split(const std::string& s, char delim) {
	std::istringstream		 iss(s);
	std::vector<std::string> elems;
	std::string				 item;
	while (std::getline(iss, item, delim))
		if (item.size())
			elems.push_back(item);
	return elems;
}

std::string pad_start(const std::string& str, size_t length, char pad) {
	std::string result;
	result.reserve(length);

	while (result.size() + str.length() < length)
		result += pad;
	result += str;
	return result;
}

Time::Time(const std::string& label)
	: _start(std::chrono::high_resolution_clock::now()),
	  _label(label) {}

Time::nanoseconds Time::end() const { // in nanoseconds
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start).count();
}

std::string Time::endString() const {
	return std::to_string(end()) + " ns";
}

std::string Time::endFormatted() const {
	return endFormatted(end());
}

std::string Time::endFormatted(Time::nanoseconds duration) const {
	long long hours = duration / 3600000000000;
	duration %= 3600000000000;
	long long minutes = duration / 60000000000;
	duration %= 60000000000;
	long long seconds = duration / 1000000000;
	duration %= 1000000000;
	long long milliseconds = duration / 1000000;
	duration %= 1000000;
	long long microseconds = duration / 1000;
	duration %= 1000;
	long long	nanoseconds = duration;

	std::string result;
	result.reserve(100);

	result += pad_start(std::to_string(hours), 2) + "h ";
	result += pad_start(std::to_string(minutes), 2) + "m ";
	result += pad_start(std::to_string(seconds), 2) + ".";
	result += pad_start(std::to_string(milliseconds), 3, '0') + " ";
	result += pad_start(std::to_string(microseconds), 3, '0') + " ";
	result += pad_start(std::to_string(nanoseconds), 3, '0') + "s";

	return result;
}

void Time::print() const {
	volatile Time::nanoseconds duration = end();
	if (_label.size())
		std::cout << _label << ": ";
	std::cout << endFormatted(duration) << std::endl;
}

namespace _rand {

float _float(float a, float b) {
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

Vec3 vec3(float a, float b) {
	return Vec3(_float(a, b), _float(a, b), _float(a, b));
}

} // namespace _rand
