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
  
}
