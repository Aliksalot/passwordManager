#pragma once
#include"./Cypher.h"
#include"./CipherTypes.h"

namespace encrypt {
  class TestCypher: public Cypher {
  public:
    TestCypher(const clib::String& secretword);
    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;
    TestCypher* clone() const override;
    CipherType type() const override;
    clib::String serialize() const override;
  private:
    clib::String secretword;
  };

  TestCypher::TestCypher(const clib::String& secretword): secretword(secretword) { }

  CipherType TestCypher::type() const { 
    return CipherType::TestCipher;
  }

  clib::String TestCypher::serialize() const {
    return secretword;
  }

  clib::String TestCypher::encrypt(const clib::String& text) const{
    return text + secretword;
  };
  clib::String TestCypher::decrypt(const clib::String& text) const{
    return text;
  };
  TestCypher* TestCypher::clone() const {
    return new TestCypher(secretword);
  }
  
}
