#pragma once
#include "poller.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#define MAX_BUFFER_SIZE 1024

struct Socket {
  public:
	Socket();
	Socket(int fd);
	Socket(Socket&& other);
	Socket& operator=(Socket&& other);

	void	read(std::vector<uint8_t>& buffer, size_t size);
	void	write(const std::vector<uint8_t>& buffer);

	int		fd;

	~Socket();
	Socket(const Socket& other) = delete;
	Socket& operator=(const Socket& other) = delete;

  private:
	void close();
};

class Client_socket : public Socket {
  public:
	Client_socket() = delete;
	Client_socket(const std::string& host, uint16_t port) {
		std::cout << fd << std::endl;
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);

		if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0)
			exit_with::perror("inet_pton failed");

		if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
			exit_with::perror("connect failed");

		std::cout << "client connected" << std::endl;
	}

	// ~Client_socket();
};

// todo make sure deserialize is called before accessing
class Transfer_object {
  public:
	virtual std::vector<uint8_t> serialize() const = 0;
	[[nodiscard]] virtual bool	 deserialize(const std::vector<uint8_t>& data) = 0;
};

template <typename T_request, typename T_response>
class RPC_client {
  public:
	RPC_client(const std::string& host, uint16_t port) : socket(host, port){};

	T_response call(const T_request& request) {
		this->write_buffer = request.serialize();
		socket.write(this->write_buffer);

		this->read_buffer.resize(this->write_buffer.size());
		socket.read(this->read_buffer, this->write_buffer.size());

		T_response out;
		assert(out.deserialize(this->read_buffer));
		return out;
	};

  private:
	Client_socket		 socket;
	std::vector<uint8_t> read_buffer;
	std::vector<uint8_t> write_buffer;
};

template <typename T_request, typename T_response>
class RPC_server {
  public:
	RPC_server(uint16_t port) : poller(mode_ipv4, "127.0.0.1", port){};

	typedef struct {
		fd_t	  fd;
		T_request data;
	} RPC_request;

	typedef struct {
		fd_t	   fd;
		T_response data;
	} RPC_response;

	RPC_request receive() {
		while (true) {
			for (const auto& [fd, buffer] : poller.buffers()) {
				if (poller.get_fd_type(fd) == Fd_type::RESPONDING) {
					continue;
				}

				T_request request;
				if (request.deserialize(buffer.data())) {
					poller.set_buffer(fd, {}, Fd_type::RECEIVING);
					return {fd, request};
				}
			}
			poller.poll();
		}
	}

	void respond(const RPC_response& response) {
		auto buffer = response.data.serialize();

		poller.set_buffer(response.fd, buffer, Fd_type::RESPONDING);
	}

  private:
	Poller poller;
};
