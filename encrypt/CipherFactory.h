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
        CipherConfig conf
    ) {
      switch(conf.type) {
        case CipherType::TestCipher: {
          if(conf.password != "") {
            return new TestCypher(TestCypher::fromPassword(conf.password)); 
          }
          return new TestCypher(conf.args[0]); 
        }
        case CipherType::CaesarCipher: {
          if(conf.password != "") {
            return new CaesarCipher(CaesarCipher::fromPassword(conf.password)); 
          }
          return new CaesarCipher(conf.args[0].toInt());
        }
        default: throw utils::InvalidCipherTypeException();
      }
    }
  };
}
