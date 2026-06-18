#pragma once

namespace utils {
  inline clib::Text encryptFile(const clib::Text& raw, clib::Text password)  {
    if(password.trimInPlace(' ').empty()) {
      throw std::runtime_error("Empty password is not permitted");
    }
    clib::Text out = raw;

    for(std::size_t i = 0; i < raw.size(); i ++) {
      char k = password[i % password.size()];
      out[i] = raw[i] ^ k;
    }

    return out;
  }

  inline clib::Text decryptFile(const clib::Text& raw, const clib::Text& password) {
    return encryptFile(raw, password);
  }
}
