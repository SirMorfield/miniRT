#include "poller.hpp"
#include "rpc.hpp"
#include "transfer_types.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <string>

void print_error() {
	std::cout << "Usage: server <port>" << std::endl;
	std::cout << "Usage: client <IPV4 address> <port>" << std::endl;
	std::cout << "Usage: test <IPV4 address> <port>" << std::endl;
}

// compute result
Response computer(const Request& request) {
	const Response response(request.number);
	return response;
}

void server(uint16_t port) {
	Poller poller(mode_ipv4, "127.0.0.1", port);
	poller.start();
}

void client(const std::string& host, uint16_t port) {
	std::cout << "client" << std::endl;

	std::vector<uint8_t> buffer = {'h', 'e', 'l', 'l', 'o'};
	Client_socket		 client_socket(host, port);
	while (true) {
		client_socket.write(buffer);
		std::cout << "sent" << std::endl;
		client_socket.read(buffer, buffer.size());
		std::cout << "received: ";
		for (auto c : buffer) {
			std::cout << c;
		}
		std::cout << std::endl;
	}
}

int main(int argc, const char* argv[]) {
	if (argc == 1) {
		print_error();
		return 1;
	}
	else if (!strcmp(argv[1], "server") && argc == 3) {
		server(atoi(argv[2]));
	}
	else if (!strcmp(argv[1], "client") && argc == 4) {
		client(argv[2], atoi(argv[3]));
	}
	else if (!strcmp(argv[1], "test") && argc == 4) {
		while (true) {
			RPC_client<Request, Response> client(argv[2], atoi(argv[3]));
			client.call(Request(1));
			std::cout << "received" << std::endl;
		}
	}
	else {
		print_error();
		return 1;
	}
	return 0;
}
