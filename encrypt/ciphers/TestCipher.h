#pragma once

#include"../Cipher.h"
#include"../CipherFactory.h"
#include"../CipherTypeUtils.h"

namespace encrypt {
  class TestCipher: public Cipher {
  public:
    TestCipher(const clib::Text& secretword);
    clib::Text encrypt(const clib::Text& text) const override;
    clib::Text decrypt(const clib::Text& text) const override;
    TestCipher* clone() const override;
    clib::Text type() const override;
    clib::Text serialize() const override;
    static clib::Text fromPassword(const clib::Text& password);
  private:
    clib::Text secretword;
  };

  class TestCipherFactory: public CipherFactory {
  public:
    TestCipher* fromArgs(const clib::darray<clib::Text>& args) const override;
    TestCipher* fromShell(gui::Shell& shell) const override;
    clib::Text type() const override;
  };
}
