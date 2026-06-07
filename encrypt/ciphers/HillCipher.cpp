#include"HillCipher.h"

#include"../Cipher.h"
#include"../../math/SqMatrix.h"
#include"../../math/RemRing.h"

namespace encrypt {
  HillCipher::HillCipher(math::SqMatrix m): m(m) { };

  clib::String HillCipher::serialize() const {
    return m.serialize();
  }

  clib::String HillCipher::type() const {
    return "hill_cipher";
  }

  HillCipher* HillCipher::clone() const {
    return new HillCipher(m);
  }

  clib::String HillCipher::encrypt(const clib::String& text) const {
    return fromZ26(m.apply(toZ26(text)));
  }

  clib::String HillCipher::decrypt(const clib::String& text) const {
    return fromZ26(m.applyInverse(toZ26(text)));
  }

  clib::List<math::Z26> HillCipher::toZ26(const clib::String& text) {
    clib::List<math::Z26> res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      char c = text[i];
      if(c < 'a' || c > 'z')
        throw std::runtime_error("HillCipher alphabet consists of only a-z lowercase!");
      res.add(math::Z26(signed(c - 'a')));
    }
    return res;
  }

  clib::String HillCipher::fromZ26(const clib::List<math::Z26> tokens) {
    clib::String res;
    for(auto& code: tokens) {
      res += char(code.raw() + 'a');
    }
    return res;
  }


  HillCipher* HillCipherFactory::fromArgs(const clib::List<clib::String>& args) const {
    return new HillCipher(math::SqMatrix::deserialize(args[0]));
  }
  HillCipher* HillCipherFactory::fromCin() const {
    throw std::logic_error("TODO");
  }
  clib::String HillCipherFactory::type() const {
    return "hill_cipher";
  }
}
