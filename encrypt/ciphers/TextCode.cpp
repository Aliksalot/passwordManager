#include"TextCode.h"
#include"../Cipher.h"
#include"../CipherFactory.h"
#include<fstream>
#include<iostream>
#include"../../utils/mystring.h"
#include"../../utils/darray.h"
#include"../../utils/exceptions.h"
#include"../../gui/Shell.h"

namespace encrypt {
  TextCode::TextCode(clib::Text path): path(path) { };
  TextCode::TextCode(const char* path): TextCode(clib::Text(path)) { };

  CharMap TextCode::buildLetterToIndex() const {
    CharMap cm;

    for(std::size_t i = 0; i < 256; i ++) cm.data[i] = -1;
    
    clib::Text cipherText = readCipherFile();
    for(std::size_t i = 0; i < cipherText.size(); i ++)
      if(cm.data[unsigned(cipherText[i])] < 0) cm.data[unsigned(cipherText[i])] = i;

    return cm;
  }

  clib::Text TextCode::readCipherFile() const {
    std::ifstream file(path.raw());

    if(!file)
      throw utils::FileError(clib::Text("TextCode is looking for missing file at ") + path);

    char c;
    clib::Text fileContent;

    while(file.get(c))
      fileContent += c;

    return fileContent;
  }

  clib::Text TextCode::encrypt(const clib::Text& text) const { 
    CharMap cm = buildLetterToIndex();
    clib::Text encrypted;
    for(std::size_t i = 0; i < text.size(); i ++) {
      if(cm.data[unsigned(text[i])] < 0)
        throw utils::EncryptionError("TextCode cipher file doesn't contain all neccesery letters to\
            encrypt text!");

      encrypted += clib::Text::fromInt(cm.data[unsigned(text[i])]) + ",";
    }
    return encrypted;
  }
  clib::Text TextCode::decrypt(const clib::Text& encrypted) const {
    clib::Text decrypted;
    clib::Text cipherText = readCipherFile();
    clib::darray<clib::Text> encr_indexes = encrypted.split(',');

    for(clib::Text& index: encr_indexes) {
      if(index.empty()) continue;

      decrypted += cipherText[unsigned(index.toInt())];
    }
    return decrypted;
  }

  clib::Text TextCode::serialize() const {
    return path;
  }

  clib::Text TextCode::type() const {
    return "text_code";
  }

  TextCode* TextCode::clone() const {
    return new TextCode(path);
  }

  TextCode* TextCodeFactory::fromArgs(const clib::darray<clib::Text>& args) const {
    return new TextCode(args[0]);
  }
  TextCode* TextCodeFactory::fromShell(gui::Shell& shell) const {

    shell.print_line("Provide a file, with which to encrypt your password(s): ");

    clib::Text fpath;

    clib::getLine(shell.in(), fpath);

    return new TextCode(fpath);
  }

  clib::Text TextCodeFactory::type() const {
    return "text_code";
  }
}
