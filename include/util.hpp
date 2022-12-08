#pragma once
#include "types.hpp"
#include <chrono>
#include <cmath>
#include <functional>
#include <optional>
#include <sstream>
#include <type_traits>

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
std::string				 pad_start(const std::string& str, size_t length, char pad = ' ');

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
		float			   precision = 0.1f,
		size_t			   print_precision = 5);
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
