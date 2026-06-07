#pragma once
#include"../Cipher.h"
#include"../CipherTypeUtils.h"

#define CAESAR_RANGE_ST 32
#define CAESAR_RANGE_END 126
#define CAESAR_ALPHABET_SIZE (CAESAR_RANGE_END - CAESAR_RANGE_ST + 1)

namespace encrypt {
  class CaesarCipher: public Cipher {
  public:
    CaesarCipher(unsigned step);
    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;
    CaesarCipher* clone() const override;
    CipherType type() const override;
    clib::String serialize() const override;
  private:
    const unsigned step;
  };
}

