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

    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;

    CaesarCipher* clone() const override;

    clib::String type() const override;
    clib::String serialize() const override;
  private:
    const unsigned step;
  };

  class CaesarCipherFactory: public CipherFactory {
  public:
    CaesarCipher* fromArgs(const clib::List<clib::String>& args) const override;
    CaesarCipher* fromShell(gui::Shell& shell) const override;
    clib::String type() const override;
  };
}

