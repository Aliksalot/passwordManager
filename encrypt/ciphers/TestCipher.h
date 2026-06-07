#pragma once

#include"../Cipher.h"
#include"../CipherFactory.h"
#include"../CipherTypeUtils.h"

namespace encrypt {
  class TestCipher: public Cipher {
  public:
    TestCipher(const clib::String& secretword);
    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;
    TestCipher* clone() const override;
    clib::String type() const override;
    clib::String serialize() const override;
    static clib::String fromPassword(const clib::String& password);
  private:
    clib::String secretword;
  };

  class TestCipherFactory: public CipherFactory {
  public:
    TestCipher* fromArgs(const clib::List<clib::String>& args) const override;
    TestCipher* fromCin() const override;
    clib::String type() const override;
  };
}
