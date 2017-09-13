#pragma once
#ifndef SINI_IO_H
#define SINI_IO_H

#include <fstream>
#include <string>

namespace sini {

	std::string readTextFile(const char* file_path) noexcept;

} // namespace sini

#endif // !SINI_IO_H