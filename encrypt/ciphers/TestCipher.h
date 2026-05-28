#pragma once
#include"../Cipher.h"
#include"../CipherTypeUtils.h"

namespace encrypt {
  class TestCipher: public Cipher {
  public:
    TestCipher(const clib::String& secretword);
    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;
    TestCipher* clone() const override;
    CipherType type() const override;
    clib::String serialize() const override;
    static clib::String fromPassword(const clib::String& password);
  private:
    clib::String secretword;
  };

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
