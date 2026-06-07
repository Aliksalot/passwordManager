#pragma once
#include"../utils/string.h"
#include"../utils/exceptions.h"

namespace encrypt{
  enum class CipherType {
    TestCipher,
    CaesarCipher,
    TextCode,
    HillCipher,
    VignereCipher,
  };
  inline clib::String cipherTypeToString (encrypt::CipherType t) {
    switch(t) {
      case CipherType::TestCipher: return clib::String("test_cipher");
      case CipherType::CaesarCipher: return clib::String("caesar_cipher");
      case CipherType::TextCode: return clib::String("text_code");
      case CipherType::HillCipher: return clib::String("hill_cipher");
      case CipherType::VignereCipher: return clib::String("vignere_cipher");
    }

    throw std::logic_error("unreachable");
  }
  inline CipherType stringToCipherType (const clib::String& s) {
    if(s == cipherTypeToString(CipherType::TestCipher))
      return CipherType::TestCipher;
    if(s == cipherTypeToString(CipherType::CaesarCipher))
      return CipherType::CaesarCipher;
    if(s == cipherTypeToString(CipherType::TextCode))
      return CipherType::TextCode;
    if(s == cipherTypeToString(CipherType::HillCipher))
      return CipherType::HillCipher;
    if(s == cipherTypeToString(CipherType::VignereCipher))
      return CipherType::VignereCipher;

    throw utils::InvalidCipherTypeError(clib::String("Invalid cipher: ") + s);
  }
}
