#pragma once
#include"../utils/string.h"
#include"../utils/exceptions.h"

namespace encrypt{
  enum class CipherType {
    TestCipher,
    CaesarCipher
  };
  inline clib::String cipherTypeToString (encrypt::CipherType t) {
    switch(t) {
      case encrypt::CipherType::TestCipher: return clib::String("test_cypher");
      case encrypt::CipherType::CaesarCipher: return clib::String("caesar_cypher");
      default: 
        throw utils::InvalidCipherTypeException();
    }
  }
  inline encrypt::CipherType stringToCipherType (const clib::String& s) {
    if(s == cipherTypeToString(encrypt::CipherType::TestCipher))
      return encrypt::CipherType::TestCipher;
    if(s == cipherTypeToString(encrypt::CipherType::CaesarCipher))
      return encrypt::CipherType::CaesarCipher;

    throw utils::InvalidCipherTypeException("INVALID CIPHER");
  }
}
