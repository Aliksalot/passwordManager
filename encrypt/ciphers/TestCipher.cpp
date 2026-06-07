#include"TestCipher.h"

namespace encrypt {
  TestCipher::TestCipher(const clib::String& secretword): secretword(secretword) { }

  CipherType TestCipher::type() const { 
    return CipherType::TestCipher;
  }

  clib::String TestCipher::serialize() const {
    return secretword;
  }

  clib::String TestCipher::encrypt(const clib::String& text) const{
    return text + secretword;
  };
  clib::String TestCipher::decrypt(const clib::String& text) const{
    return text;
  };
  TestCipher* TestCipher::clone() const {
    return new TestCipher(secretword);
  }

  clib::String TestCipher::fromPassword(const clib::String& password) {
    return password;
  }
}
