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
