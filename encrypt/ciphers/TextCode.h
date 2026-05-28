#pragma once

#include"../Cipher.h"
#include<fstream>
#include"../../utils/string.h"
#include"../../utils/array.h"
#include"../../utils/exceptions.h"

namespace encrypt {

  struct CharMap {
    signed data[256];
  };

  class TextCode: public Cipher{
  private:

    clib::String path;
    
    CharMap buildLetterToIndex() const;
    
    clib::String readCipherFile() const;
  public:

    TextCode() = delete;
    TextCode(clib::String path);
    TextCode(const char* path);

    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;

    clib::String serialize() const override;
    CipherType type() const override;

    TextCode* clone() const override;
  };


  inline TextCode::TextCode(clib::String path): path(path) { };
  inline TextCode::TextCode(const char* path): TextCode(clib::String(path)) { };

  inline CharMap TextCode::buildLetterToIndex() const {
    CharMap cm;

    for(std::size_t i = 0; i < 256; i ++) cm.data[i] = -1;
    
    clib::String cipherText = readCipherFile();
    for(std::size_t i = 0; i < cipherText.size(); i ++)
      if(cm.data[cipherText[i]] < 0) cm.data[cipherText[i]] = i;

    return cm;
  }

  inline clib::String TextCode::readCipherFile() const {
    std::ifstream file(path.raw());

    if(!file)
      throw utils::FileError(clib::String("TextCode is looking for missing file at ") + path);

    char c;
    clib::String fileContent;

    while(file.get(c))
      fileContent += c;

    return fileContent;
  }

  inline clib::String TextCode::encrypt(const clib::String& text) const { 
    CharMap cm = buildLetterToIndex();
    clib::String encrypted;
    for(std::size_t i = 0; i < text.size(); i ++) {
      if(cm.data[unsigned(text[i])] < 0)
        throw utils::EncryptionError("TextCode cipher file doesn't contain all neccesery letters to\
            encrypt text!");

      encrypted += clib::String::fromInt(cm.data[unsigned(text[i])]) + ",";
    }
    return encrypted;
  }
  inline clib::String TextCode::decrypt(const clib::String& encrypted) const {
    clib::String decrypted;
    clib::String cipherText = readCipherFile();
    clib::List<clib::String> encr_indexes = encrypted.split(',');

    for(clib::String& index: encr_indexes) {
      if(index.empty()) continue;

      decrypted += cipherText[unsigned(index.toInt())];
    }
    return decrypted;
  }

  inline clib::String TextCode::serialize() const {
    return path;
  }

  inline CipherType TextCode::type() const {
    return CipherType::TextCode;
  }

  inline TextCode* TextCode::clone() const {
    return new TextCode(path);
  }
}
