#pragma once

#include"../../utils/array.h"
#include"../../utils/string.h"
#include"../../math/RemRing.h"
#include"../Cipher.h"

namespace encrypt {
  class VignereCipher : public Cipher {
  private:
    clib::List<math::Z96> key;
  public:
    VignereCipher(const clib::String& key);
    VignereCipher(const clib::List<math::Z96>& key);

    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;

    clib::String serialize() const override;
    CipherType type() const override;

    VignereCipher* clone() const override;

    static clib::List<math::Z96> toZ96(const clib::String& text);
    static clib::String fromZ96(const clib::List<math::Z96> tokens);
  };
}
