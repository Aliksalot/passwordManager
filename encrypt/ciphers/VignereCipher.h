#pragma once

#include"../../utils/array.h"
#include"../../utils/string.h"
#include"../../math/Z26.h"
#include"../Cipher.h"

namespace encrypt {
  class VignereCipher : public Cipher {
  private:
    clib::List<math::Z26> key;
  public:
    VignereCipher(const clib::String& key);
    VignereCipher(const clib::List<math::Z26>& key);

    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;

    clib::String serialize() const override;
    CipherType type() const override;

    VignereCipher* clone() const override;
  };
}
