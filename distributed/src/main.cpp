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
	RPC_server<Request, Response>				poller(port);
	RPC_server<Request, Response>::RPC_response response;

	while (true) {
		auto request = poller.receive();
		std::cout << "received request: " << std::endl;
		response = {request.fd, Response(42)};
		poller.respond(response);
		std::cout << "responded " << std::endl;
	}
}

void client(const std::string& host, uint16_t port) {
	std::vector<uint8_t> buffer_write = {'a', 'b', 'v', '*'};
	std::vector<uint8_t> buffer_reaad = {'a', 'a', 'a', '*'};
	Client_socket		 client_socket(host, port);
	client_socket.write(buffer_write);

	// while (true) {
	// 	client_socket.write(buffer_write);
	// 	client_socket.read(buffer_reaad, buffer_reaad.size());
	// }
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
