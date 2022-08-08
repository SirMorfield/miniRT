#pragma once
#include "types.hpp"
#include <chrono>
#include <functional>
#include <optional>
#include <sstream>

template <class T>
T radians(T degrees) {
	return degrees * 3.141592653589793 / 180;
}

template <typename T>
T degrees(T radians) {
	return radians * 180 / 3.141592653589793;
}

ID generateID();

template <typename T>
std::optional<T> parse_int(const std::string& str) {
	char			  c;
	std::stringstream ss(str);
	T				  result;

	ss >> result;
	if (ss.fail() || ss.get(c))
		return {};
	else
		return result;
}

//
std::vector<std::string> split(const std::string& s, char delim);
// n

std::string pad_start(const std::string& str, size_t length, char pad = ' ');

class Time {
  public:
	typedef long long nanoseconds;

	Time();

	Time::nanoseconds end() const;
	std::string		  endString() const;
	std::string		  endFormatted() const;
	std::string		  endFormatted(Time::nanoseconds duration) const;

	void			  print() const;

  private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	//
};

namespace _rand {

float _float(float a, float b);
Vec3  vec3(float a, float b);

} // namespace _rand
