#pragma once

#include"../utils/array.h"
#include"../utils/string.h"
#include"../utils/exceptions.h"

#include"./ciphers/TestCipher.h"
#include"./ciphers/CaesarCipher.h"
#include"./ciphers/TextCode.h"
#include"./ciphers/HillCipher.h"
#include"./ciphers/VignereCipher.h"

#include"../math/SqMatrix.h"

namespace encrypt {

  class CipherFactory {
  public:
    static Cipher* create(
        CipherType type,
        const clib::List<clib::String>& args
    ) {
      switch(type) {
        case CipherType::TestCipher: {
          return new TestCipher(args[0]); 
        }
        case CipherType::CaesarCipher: {
          return new CaesarCipher(args[0].toInt());
        }
        case CipherType::TextCode: {
          return new TextCode(args[0]);
        }
        case CipherType::HillCipher: {
          return new HillCipher(math::SqMatrix::deserialize(args[0]));
        }
        case CipherType::VignereCipher: {
          return new VignereCipher(args[0]);
        }
      }

      throw std::logic_error("unreachable");
    }
  };
}
