#pragma once

#include"../Cipher.h"
#include"../CipherTypeUtils.h"
#include"../../math/SqMatrix.h"
#include"../../math/Z26.h"

namespace encrypt {

  class HillCipher: public Cipher {
  private:
    math::SqMatrix m;
  public:
     clib::String encrypt(const clib::String& text) const override;
     clib::String decrypt(const clib::String& text) const override;

     clib::String serialize() const override;
     CipherType type() const override;

     HillCipher* clone() const override;

     HillCipher() = delete;
     HillCipher(math::SqMatrix m);

  };

  inline HillCipher::HillCipher(math::SqMatrix m): m(m) { };

  inline clib::String HillCipher::serialize() const {
    return m.serialize();
  }

  inline CipherType HillCipher::type() const {
    return CipherType::HillCipher;
  }

  inline HillCipher* HillCipher::clone() const {
    return new HillCipher(m);
  }

  inline clib::String HillCipher::encrypt(const clib::String& text) const {
    return math::Z26::fromZ26(m.apply(math::Z26::toZ26(text)));
  }

  inline clib::String HillCipher::decrypt(const clib::String& text) const {
    return math::Z26::fromZ26(m.applyInverse(math::Z26::toZ26(text)));
  }

}
