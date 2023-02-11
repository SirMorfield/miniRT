#pragma once
#include <arpa/inet.h>
#include <array>
#include <cassert>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <limits>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#ifdef __APPLE__
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#else
#include <sys/sendfile.h>
#endif
