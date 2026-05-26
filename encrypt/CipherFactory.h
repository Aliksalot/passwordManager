#pragma once
#include"../utils/array.h"
#include"../utils/string.h"
#include"../utils/exceptions.h"

namespace encrypt {

  class CipherFactory {
  public:
    static Cypher* create(
        CipherType type,
        const clib::List<clib::String>& conf
    ) {

      switch(type) {
        case CipherType::TestCipher: return new encrypt::TestCypher(conf[0]);
        default: throw utils::InvalidCipherTypeException();
      }
    }
  };
}
