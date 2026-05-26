#pragma once

#include <stdexcept>

namespace utils {
  class InvalidCipherTypeException : public std::runtime_error {
  public:
      InvalidCipherTypeException(): std::runtime_error("Invalid CipherType") {}
      InvalidCipherTypeException(const char* s): std::runtime_error(s) {}
  };
}
