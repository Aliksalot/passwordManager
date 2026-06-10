#pragma once
#include"../utils/mystring.h"
#include"../utils/exceptions.h"
#include<iostream>

namespace encrypt{
  enum class CipherType {
    TestCipher,
    CaesarCipher,
    TextCode,
    HillCipher,
    VignereCipher,
  };
  inline clib::Text cipherTypeToText (encrypt::CipherType t) {
    switch(t) {
      case CipherType::TestCipher:    return clib::Text("test_cipher");
      case CipherType::CaesarCipher:  return clib::Text("caesar_cipher");
      case CipherType::TextCode:      return clib::Text("text_code");
      case CipherType::HillCipher:    return clib::Text("hill_cipher");
      case CipherType::VignereCipher: return clib::Text("vignere_cipher");
    }

    throw std::logic_error("unreachable");
  }
  inline CipherType stringToCipherType (const clib::Text& s) {
    if(s == cipherTypeToText(CipherType::TestCipher))
      return CipherType::TestCipher;
    if(s == cipherTypeToText(CipherType::CaesarCipher))
      return CipherType::CaesarCipher;
    if(s == cipherTypeToText(CipherType::TextCode))
      return CipherType::TextCode;
    if(s == cipherTypeToText(CipherType::HillCipher))
      return CipherType::HillCipher;
    if(s == cipherTypeToText(CipherType::VignereCipher))
      return CipherType::VignereCipher;

    throw utils::InvalidCipherTypeError(clib::Text("Invalid cipher: ") + s);
  }
}
