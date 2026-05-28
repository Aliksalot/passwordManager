#pragma once
#include"./Cypher.h"
#include"./CipherTypes.h"

namespace encrypt {
  class CaesarCipher: public Cypher {
  public:
    CaesarCipher(unsigned key);
    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;
    CaesarCipher* clone() const override;
    CipherType type() const override;
    clib::String serialize() const override;
  private:
    const unsigned key;
  };

  CaesarCipher::CaesarCipher(unsigned key): key(key) { }

  CipherType CaesarCipher::type() const { 
    return CipherType::CaesarCipher;
  }

  clib::String CaesarCipher::serialize() const {
    return clib::String(key + "");
  }

  clib::String CaesarCipher::encrypt(const clib::String& text) const{
    return text;
  };
  clib::String CaesarCipher::decrypt(const clib::String& text) const{
    return text;
  };
  CaesarCipher* CaesarCipher::clone() const {
    return new CaesarCipher(key);
  }
  
}

