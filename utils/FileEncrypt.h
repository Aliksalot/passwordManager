#pragma once

namespace utils {

  class FileEncrypt {

  public:
    static clib::String encrypt(const clib::String& raw, const clib::String& password);

    static clib::String decrypt(const clib::String& raw, const clib::String& password);

  };

  inline clib::String FileEncrypt::encrypt(const clib::String& raw, const clib::String& password)  {
    clib::String out = raw;

    for(std::size_t i = 0; i < raw.size(); i ++) {
      char k = password[i % password.size()];
      out[i] = raw[i] ^ k;
    }

    return out;
  }
  inline clib::String FileEncrypt::decrypt(const clib::String& raw, const clib::String& password) {
    return encrypt(raw, password);
  }
}
