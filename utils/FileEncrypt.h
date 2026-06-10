#pragma once

namespace utils {
  inline clib::String encryptFile(const clib::String& raw, clib::String password)  {
    if(password.trim(' ').empty()) {
      throw std::runtime_error("Empty password is not permitted");
    }
    clib::String out = raw;

    for(std::size_t i = 0; i < raw.size(); i ++) {
      char k = password[i % password.size()];
      out[i] = raw[i] ^ k;
    }

    return out;
  }

  inline clib::String decryptFile(const clib::String& raw, const clib::String& password) {
    return encryptFile(raw, password);
  }
}
