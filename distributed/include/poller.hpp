#pragma once
#include "main.hpp"

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

namespace exit_with {

void perror(const std::string& msg);
void e_errno(const std::string& msg);
void message(const std::string& msg);

} // namespace exit_with
typedef int fd_t;
enum IP_mode {
	mode_ipv4,
	mode_ipv6
};

namespace constructors {

struct sockaddr_in6 sockaddr6(uint16_t port);
struct sockaddr_in	sockaddr(const char* str_addr, uint16_t port);
fd_t				server_socket(IP_mode ip_mode, const char* str_addr, uint16_t port);
struct pollfd		pollfd(int fd, short events);

} // namespace constructors

enum class Fd_type {
	SERVER,
	RECEIVING,
	RESPONDING,
	CLOSED
};

enum class IO_status {
	DONE,
	ERROR
};

class Buffer {
  public:
	Buffer() {}
	Buffer(const Buffer& cp) : fd(cp.fd) {}
	Buffer& operator=(const Buffer& mv) {
		if (this == &mv) {
			return *this;
		}
		// this->close();
		this->fd = mv.fd;
		return *this;
	}

	Buffer(fd_t fd, std::vector<uint8_t> buffer) : fd(fd), _buffer(buffer) {
		if (this->fd < 0)
			exit_with::message("Invalid socket fd");
	}
	Buffer(fd_t fd) : Buffer(fd, std::vector<uint8_t>()) {}

	IO_status read() {
		static char buf[1024];
		for (size_t read_i = 0; read_i < 10; read_i++) {
			ssize_t bytes_read = ::read(fd, buf, sizeof(buf));
			if (bytes_read < 0)
				return IO_status::DONE; // todo
			if (bytes_read == 0)
				return IO_status::DONE;
			for (ssize_t i = 0; i < bytes_read; i++) {
				_buffer.push_back(buf[i]);
			}
		}
		return IO_status::DONE;
	}

	IO_status write() {
		for (size_t i = 0; i < 10; i++) {
			ssize_t bytes_written = ::write(fd, _buffer.data(), _buffer.size());
			if (bytes_written < 0)
				return IO_status::DONE; // todo
			if (bytes_written == 0)
				return IO_status::DONE;
			_buffer.erase(_buffer.begin(), _buffer.begin() + bytes_written);
		}
		return IO_status::DONE;
	}

	const std::vector<uint8_t>& data() const { return _buffer; }

	Buffer(Buffer&& mv) = delete;
	//   private:
	fd_t				 fd;
	std::vector<uint8_t> _buffer;
};

class Poller {
  public:
	Poller() = delete;
	Poller(IP_mode ip_mode, const char* str_addr, uint16_t port) {
		const fd_t			server_socket = constructors::server_socket(ip_mode, str_addr, port);
		const struct pollfd pfd = constructors::pollfd(server_socket, POLLIN | POLLOUT);

		this->fd = pfd.fd;
		this->port = port;
		_pollfds.push_back(pfd);
		_fd_types[pfd.fd] = Fd_type::SERVER;
	}
	const std::map<fd_t, Buffer>& buffers() const { return _buffers; }
	Fd_type						  get_fd_type(fd_t f) const { return _fd_types.at(f); }
	//
	void set_buffer(fd_t client_fd, const std::vector<uint8_t>& buffer, Fd_type status) {

		_buffers.at(client_fd) = Buffer(client_fd, buffer);
		_fd_types.at(client_fd) = status;
		if (status == Fd_type::RESPONDING) {
			for (auto& pfd : _pollfds) {
				if (pfd.fd == client_fd) {
					pfd.events = POLLOUT;
					return;
				}
			}
		}

		if (status == Fd_type::RECEIVING) {
			for (auto& pfd : _pollfds) {
				if (pfd.fd == client_fd) {
					pfd.events = POLLIN;
					return;
				}
			}
		}
	}

	void poll();
	~Poller() {}

  private:
	void accept_requests();

	void on_poll(pollfd pfd);
	void on_poll(pollfd pfd, Buffer& buffer);

	void add_fd(pollfd pfd, const Buffer& buffer) {
		_pollfds.push_back(pfd);
		_fd_types[pfd.fd] = Fd_type::RECEIVING;
		_buffers[pfd.fd] = buffer;
	}

	void close_fd(pollfd pfd) {
		close(pfd.fd);
		_buffers.erase(pfd.fd);
		_fd_types[pfd.fd] = Fd_type::CLOSED;
	}

	fd_t					   fd;
	uint16_t				   port;

	std::vector<struct pollfd> _pollfds;
	std::map<fd_t, Fd_type>	   _fd_types;
	std::map<fd_t, Buffer>	   _buffers;

	// disabled
	Poller(const Poller& cp) = delete;
	Poller& operator=(const Poller& cp) = delete;
};
