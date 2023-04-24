#include "rpc.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

[[noreturn]] void exit_with_perror(const std::string& msg) {
	::perror(msg.c_str());
	exit(EXIT_FAILURE);
}

Socket::Socket() : Socket(socket(AF_INET, SOCK_STREAM, 0)) {}

Socket::Socket(int fd) : fd(fd) {
	if (this->fd < 0)
		exit_with_perror("Invalid socket fd");
}

Socket::Socket(Socket&& other) : fd(other.fd) {
	other.fd = -1;
}

Socket& Socket::operator=(Socket&& other) {
	if (this == &other) {
		return *this;
	}
	this->close();
	fd = other.fd;
	other.fd = -1;
	return *this;
}

void Socket::read(std::vector<uint8_t>& buffer, size_t size) {
	buffer.resize(size);
	ssize_t n = ::read(this->fd, buffer.data(), size);
	if (n <= 0) // TODO: buffering
		exit_with_perror("read failed");
	buffer.resize((size_t)n);
}

void Socket::write(const std::vector<uint8_t>& buffer) {
	ssize_t n = ::write(this->fd, buffer.data(), buffer.size());
	if (n != (ssize_t)buffer.size()) // TODO: buffering
		exit_with_perror("write failed");
}

void Socket::close() {
	if (fd < 0)
		return;

	if (::close(fd) < 0)
		exit_with_perror("could not close socket");
	std::cout << "closed fd" << std::endl;
	fd = -1;
}

Socket::~Socket() {
	this->close();
}
