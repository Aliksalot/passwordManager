#pragma once
#include"../utils/string.h"
#include"../utils/exceptions.h"

namespace encrypt{
  enum class CipherType {
    TestCipher,
    CaesarCipher,
    TextCode
  };
  inline clib::String cipherTypeToString (encrypt::CipherType t) {
    switch(t) {
      case CipherType::TestCipher: return clib::String("test_cypher");
      case CipherType::CaesarCipher: return clib::String("caesar_cypher");
      case CipherType::TextCode: return clib::String("text_code");
      default: 
        throw utils::InvalidCipherTypeError();
    }
  }
  inline CipherType stringToCipherType (const clib::String& s) {
    if(s == cipherTypeToString(CipherType::TestCipher))
      return CipherType::TestCipher;
    if(s == cipherTypeToString(CipherType::CaesarCipher))
      return CipherType::CaesarCipher;
    if(s == cipherTypeToString(CipherType::TextCode))
      return CipherType::TextCode;

    throw utils::InvalidCipherTypeError(clib::String("Invalid cipher: ") +s);
  }
}
