#pragma once
#include "rpc.hpp"
#include <cstring>
#include <iostream>

#define ASSERT(left, operator, right)                                                                                               \
	{                                                                                                                               \
		if (!((left) operator(right))) {                                                                                            \
			std::cerr << "ASSERTION FAILED: " << #left << " " << #operator<< " " << #right << " function " << __func__ << ", file " \
					  << "./" << __FILE__ << ":" << __LINE__ << "\n"                                                                \
					  << #left << " = " <<(left)                                                                                    \
					  << "\n"                                                                                                       \
					  << #right << " = " << (right) << std::endl;                                                                   \
			exit(EXIT_FAILURE);                                                                                                     \
		}                                                                                                                           \
	}

struct Request : public Transfer_object {
	Request(int number) : number(number) {}
	Request() {}

  public:
	std::vector<uint8_t> serialize() const {
		std::vector<uint8_t> buffer;
		buffer.resize(sizeof(number));
		std::memcpy(buffer.data(), &number, sizeof(number));
		return buffer;
	}
	[[nodiscard]] bool deserialize(const std::vector<uint8_t>& data) {
		if (data.size() != sizeof(number))
			return false;
		std::memcpy(&number, data.data(), sizeof(number));
		return true;
	}

	int number;
};

struct Response : public Transfer_object {
  public:
	Response(int number) : number(number) {}
	Response() {}

	std::vector<uint8_t> serialize() const {
		std::vector<uint8_t> buffer;
		buffer.resize(sizeof(number));
		std::memcpy(buffer.data(), &number, sizeof(number));
		return buffer;
	}

	[[nodiscard]] bool deserialize(const std::vector<uint8_t>& data) {
		if (data.size() != sizeof(number))
			return false;
		std::memcpy(&number, data.data(), sizeof(number));
		return true;
	}

	int number;
};
