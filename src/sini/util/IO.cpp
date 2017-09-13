#include "sini/util/IO.hpp"

namespace sini {

	std::string readTextFile(const char* file_path) noexcept {

		std::ifstream file{ file_path };
		std::string file_str{ std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() };

		return std::move(file_str);
	}

} // namespace sini