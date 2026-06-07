#include "VignereCipher.h"
#include "../CipherTypeUtils.h"

namespace encrypt {
  VignereCipher::VignereCipher(const clib::String& key)
   : key(math::Z26::toZ26(key)) { }

  VignereCipher::VignereCipher(const clib::List<math::Z26>& key)
   : key(key) { };

  clib::String VignereCipher::encrypt(const clib::String& text) const {
    clib::List<math::Z26> result;
    clib::List<math::Z26> tokens = math::Z26::toZ26(text);
    for(std::size_t i = 0; i < text.size(); i ++) {
      result.add(tokens[i] + key[i % key.size()]);
    }
    return math::Z26::fromZ26(result);
  }
  clib::String VignereCipher::decrypt(const clib::String& text) const {
    clib::List<math::Z26> result;
    clib::List<math::Z26> tokens = math::Z26::toZ26(text);
    for(std::size_t i = 0; i < text.size(); i ++) {
      result.add(tokens[i] - key[i % key.size()]);
    }
    return math::Z26::fromZ26(result);
  }

  clib::String VignereCipher::serialize() const {
    return math::Z26::fromZ26(key);
  }
  CipherType VignereCipher::type() const { 
    return CipherType::VignereCipher;
  }

  VignereCipher* VignereCipher::clone() const {
    return new VignereCipher(key);
  }
}
