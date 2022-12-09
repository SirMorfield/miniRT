#include "env.hpp"
#include <iostream>

namespace env {

// Allow non-normalized direction vector in .rt file? :boolean
const uint64_t threads = 4;
const bool	   allow_abnormal_dir = true;
const bool	   log_progress = true;

// 1, 4, 16, 32, ect
const uint64_t		  anti_aliasing_level = 1;

const Resolution_from read_resolution_from = Resolution_from::cpp_file;

const uint64_t		  width = 400;
const uint64_t		  height = 400;

//
void print() {
	std::cout << "Env" << std::endl;
	std::cout << "  threads             : " << threads << std::endl;
	std::cout << "  allow_abnormal_dir  : " << (allow_abnormal_dir ? "true" : "false") << std::endl;
	std::cout << "  log_progress        : " << (log_progress ? "true" : "false") << std::endl;
	std::cout << "  anti_aliasing_level : " << anti_aliasing_level << "\n " << std::endl;
}

} // namespace env
