#pragma once
#include <cstdint>

// See settings.cpp
enum class Resolution_from {
	rt_file,
	cpp_file

};

namespace env {
void print();

//
extern const bool			 allow_abnormal_dir;
extern const bool			 log_progress;
extern const uint64_t		 threads;
extern const uint64_t		 anti_aliasing_level;
extern const Resolution_from read_resolution_from;
extern const uint64_t		 width;
extern const uint64_t		 height;

} // namespace env
