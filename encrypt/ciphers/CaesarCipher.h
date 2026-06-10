#pragma once
#include"../Cipher.h"
#include"../CipherFactory.h"
#include"../CipherTypeUtils.h"

#define CAESAR_RANGE_ST 32
#define CAESAR_RANGE_END 126
#define CAESAR_ALPHABET_SIZE (CAESAR_RANGE_END - CAESAR_RANGE_ST + 1)

namespace gui {
  class Shell;
}

namespace encrypt {
  class CaesarCipher: public Cipher {
  public:
    CaesarCipher(unsigned step);

    clib::Text encrypt(const clib::Text& text) const override;
    clib::Text decrypt(const clib::Text& text) const override;

    CaesarCipher* clone() const override;

    clib::Text type() const override;
    clib::Text serialize() const override;
  private:
    const unsigned step;
  };

  class CaesarCipherFactory: public CipherFactory {
  public:
    CaesarCipher* fromArgs(const clib::darray<clib::Text>& args) const override;
    CaesarCipher* fromShell(gui::Shell& shell) const override;
    clib::Text type() const override;
  };
}

