#pragma once

#include <stdexcept>


namespace sini {

class SdlException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

}
