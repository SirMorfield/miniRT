#pragma once
#include "types.hpp"
#include <chrono>
#include <functional>
#include <optional>
#include <sstream>
#include <type_traits>

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

	Time(const std::string& label = "");
	void			  start();
	Time::nanoseconds duration() const;
	std::string		  duration_str() const;
	std::string		  duration_formatted() const;
	std::string		  duration_formatted(Time::nanoseconds duration) const;

	void			  print() const;

  protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	const std::string											_label;
};

class Progress_logger : public Time {
  public:
	Progress_logger(
		const std::string& label = "Progress",
		float			   precision = 1.0f,
		size_t			   print_precision = 7);
	void print(float percentage);

  private:
	void		 _print(float percentage, Time::nanoseconds remaining_time);
	const float	 _precision;
	const size_t _print_precision;

	float		 _last_percentage;
};

template <typename T>
bool is_power_of_2(T x, std::enable_if<std::is_integral<T>::value>* = nullptr) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

namespace _rand {

float _float(float a, float b);
Vec3  vec3(float a, float b);

} // namespace _rand
