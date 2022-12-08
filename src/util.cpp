#include "util.hpp"
#include "arithmetic.hpp"
#include "vector.hpp"
#include <cmath>
#include <iomanip>

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
	: _label(label) {
	this->start();
}

void Time::start() {
	_start = std::chrono::high_resolution_clock::now();
}

Time::nanoseconds Time::duration() const { // in nanoseconds
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start).count();
}

std::string Time::duration_str() const {
	return std::to_string(duration()) + " ns";
}

std::string Time::duration_formatted() const {
	return duration_formatted(duration());
}

Progress_logger::Progress_logger(
	const std::string& label,
	float			   precision,
	size_t			   print_precision)
	: Time(label),
	  _precision(precision),
	  _print_precision(print_precision),
	  _last_percentage(-1.0f) {
	this->start();
}

void Progress_logger::print(float percentage) {
	if (this->_last_percentage >= 100.0f)
		return;
	if (percentage >= 100.0f) {
		this->_print(100.0f, 0);
		this->_last_percentage = 100.0f;
		std::cout << std::endl;
	}

	if (percentage - this->_last_percentage < this->_precision)
		return;
	_last_percentage = percentage;
	const Time::nanoseconds remaining_time = (100.0f - percentage) * (this->duration() / percentage);
	this->_print(percentage, remaining_time);
}

void Progress_logger::_print(float percentage, Time::nanoseconds remaining_time) {
	std::cout << "\r" << _label << ": ";
	std::cout << std::fixed;
	std::cout << std::setprecision(_print_precision);
	std::cout << std::setw(4 + this->_print_precision);
	std::cout << percentage << "%, ";
	std::cout << this->duration_formatted(remaining_time) << " remaining";
	std::cout << std::ends;
	std::flush(std::cout);
}

std::string Time::duration_formatted(Time::nanoseconds duration) const {
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
	volatile Time::nanoseconds duration = this->duration();
	if (_label.size())
		std::cout << _label << ": ";
	std::cout << this->duration_formatted(duration) << std::endl;
}
