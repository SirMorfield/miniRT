#include "poller.hpp"
#include "main.hpp"
#include <fcntl.h>
#include <limits>
#include <netinet/in.h>
#include <sstream>
#include <sys/ioctl.h>
#include <sys/poll.h>

#ifdef __APPLE__
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#else
#include <sys/sendfile.h>
#endif

void Poller::accept_requests() {
	while (true) {
		struct sockaddr address;
		socklen_t		address_len = sizeof(address);
		int				newfd = accept(this->fd, &address, &address_len);

		if (newfd < 0 && errno != EWOULDBLOCK)
			exit_with::perror("accept() failed");
		if (newfd < 0)
			break;
		if (fcntl(newfd, F_SETFL, O_NONBLOCK) == -1)
			exit_with::perror("Cannot set non blocking");

		const struct pollfd client_pfd = constructors::pollfd(newfd, POLLIN | POLLOUT);
		add_fd(client_pfd, Buffer(client_pfd.fd));
	}
}

void Poller::on_poll(pollfd pfd, Buffer& buffer) {
	if (pfd.revents & POLLIN)
		buffer.read();
	if (pfd.revents & POLLOUT)
		buffer.write();
}

void Poller::on_poll(pollfd pfd) {
	switch (_fd_types[pfd.fd]) {
	case Fd_type::SERVER:
		accept_requests();
		break;

	case Fd_type::RESPONSE:
	case Fd_type::REQUEST:
		on_poll(pfd, _buffers[pfd.fd]);
		break;
	default:
		break;
	}
}

void Poller::start() {
	std::cout << "listening" << std::endl;
	while (true) {
		int rc = poll(_pollfds.data(), _pollfds.size(), -1);
		if (rc < 0)
			exit_with::perror("poll() failed");
		if (rc == 0)
			exit_with::perror("poll() timeout");
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (_pollfds[i].revents == 0)
				continue;
			on_poll(_pollfds[i]);
		}
		// removing closed fds from array by shifting them to the left
		std::vector<struct pollfd>::iterator valid = _pollfds.begin();
		std::vector<struct pollfd>::iterator current = _pollfds.begin();
		while (current != _pollfds.end()) {
			if (_fd_types.find(current->fd) == _fd_types.end() || _fd_types.at(current->fd) == Fd_type::CLOSED) {
				_fd_types.erase(current->fd);
				++current;
				continue;
			}
			*valid++ = *current++;
		}
		_pollfds.erase(valid, _pollfds.end());
	}
}
