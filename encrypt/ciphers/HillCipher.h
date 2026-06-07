#pragma once

#include"../Cipher.h"
#include"../CipherTypeUtils.h"
#include"../../math/SqMatrix.h"
#include"../../math/RemRing.h"

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

     static clib::List<math::Z26> toZ26(const clib::String& text);
     static clib::String fromZ26(const clib::List<math::Z26> tokens);
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
    return fromZ26(m.apply(toZ26(text)));
  }

  inline clib::String HillCipher::decrypt(const clib::String& text) const {
    return fromZ26(m.applyInverse(toZ26(text)));
  }

  inline clib::List<math::Z26> HillCipher::toZ26(const clib::String& text) {
    clib::List<math::Z26> res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      char c = text[i];
      if(c < 'a' || c > 'z')
        throw std::runtime_error("HillCipher alphabet consists of only a-z lowercase!");
      res.add(math::Z26(signed(c - 'a')));
    }
    return res;
  }

  inline clib::String HillCipher::fromZ26(const clib::List<math::Z26> tokens) {
    clib::String res;
    for(auto& code: tokens) {
      res += char(code.raw() + 'a');
    }
    return res;
  }

}
