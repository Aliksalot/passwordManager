#pragma once

#include"../utils/array.h"
#include"../utils/string.h"
#include"../utils/exceptions.h"
#include"./CipherConfig.h"

#include"./TestCypher.h"
#include"./CaesarCipher.h"

namespace encrypt {

  class CipherFactory {
  public:
    static Cypher* create(
        CipherType type,
        const clib::List<clib::String>& args
    ) {
      switch(type) {
        case CipherType::TestCipher: {
          return new TestCypher(args[0]); 
        }
        case CipherType::CaesarCipher: {
          return new CaesarCipher(args[0].toInt());
        }
        default: throw utils::InvalidCipherTypeException();
      }
    }
  };
}
