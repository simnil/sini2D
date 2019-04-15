#pragma once

#include <stdexcept>


namespace sini {

class OpenGlException : public std::runtime_error {
    using runtime_error::runtime_error;
};

}
