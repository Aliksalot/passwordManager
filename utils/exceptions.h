#pragma once

#include"./string.h"
#include <stdexcept>

namespace utils {
  class FileError : public std::runtime_error {
  public:
    FileError(): std::runtime_error("Couldn't read file.") {}
    FileError(const clib::String& s): std::runtime_error(
      (clib::String("Couldn't read file: ") + s).raw()
    ) {}
    FileError(const char* s): FileError(clib::String(s)) {}
  };
    
  class EncryptionError : public std::runtime_error {
  public:
    EncryptionError(): std::runtime_error("Couldn't encrypt/decrypt file.") {}
    EncryptionError(const clib::String& s): std::runtime_error(
      (clib::String("Couldn't encrypt/decrypt file: ") + s).raw()
    ) {}
    EncryptionError(const char* s): EncryptionError(clib::String(s)) {}
  };

  class InvalidCipherTypeError : public EncryptionError {
  public:
      InvalidCipherTypeError(): EncryptionError("Invalid CipherType") {}
      InvalidCipherTypeError(const char* s): EncryptionError(s) {}
      InvalidCipherTypeError(const clib::String& s): EncryptionError(s.raw()) {}
  };
}
