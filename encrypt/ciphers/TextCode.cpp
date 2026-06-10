#include"TextCode.h"
#include"../Cipher.h"
#include"../CipherFactory.h"
#include<fstream>
#include<iostream>
#include"../../utils/string.h"
#include"../../utils/array.h"
#include"../../utils/exceptions.h"

namespace encrypt {
  TextCode::TextCode(clib::String path): path(path) { };
  TextCode::TextCode(const char* path): TextCode(clib::String(path)) { };

  CharMap TextCode::buildLetterToIndex() const {
    CharMap cm;

    for(std::size_t i = 0; i < 256; i ++) cm.data[i] = -1;
    
    clib::String cipherText = readCipherFile();
    for(std::size_t i = 0; i < cipherText.size(); i ++)
      if(cm.data[unsigned(cipherText[i])] < 0) cm.data[unsigned(cipherText[i])] = i;

    return cm;
  }

  clib::String TextCode::readCipherFile() const {
    std::ifstream file(path.raw());

    if(!file)
      throw utils::FileError(clib::String("TextCode is looking for missing file at ") + path);

    char c;
    clib::String fileContent;

    while(file.get(c))
      fileContent += c;

    return fileContent;
  }

  clib::String TextCode::encrypt(const clib::String& text) const { 
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
  clib::String TextCode::decrypt(const clib::String& encrypted) const {
    clib::String decrypted;
    clib::String cipherText = readCipherFile();
    clib::List<clib::String> encr_indexes = encrypted.split(',');

    for(clib::String& index: encr_indexes) {
      if(index.empty()) continue;

      decrypted += cipherText[unsigned(index.toInt())];
    }
    return decrypted;
  }

  clib::String TextCode::serialize() const {
    return path;
  }

  clib::String TextCode::type() const {
    return "text_code";
  }

  TextCode* TextCode::clone() const {
    return new TextCode(path);
  }

  TextCode* TextCodeFactory::fromArgs(const clib::List<clib::String>& args) const {
    return new TextCode(args[0]);
  }
  TextCode* TextCodeFactory::fromCin() const {

    std::cout << "Provide a file, with which to encrypt your password(s): " << std::endl;

    clib::String fpath;
    char c = 0;

    while(std::cin.get(c) && c != '\n') {
      fpath += c;
    }

    return new TextCode(fpath);
  }

  clib::String TextCodeFactory::type() const {
    return "text_code";
  }
}
