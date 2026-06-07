#pragma once

#include"../Cipher.h"
#include<fstream>
#include"../../utils/string.h"
#include"../../utils/array.h"
#include"../../utils/exceptions.h"

#include"../CipherFactory.h"

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
    clib::String type() const override;

    TextCode* clone() const override;
  };

  class TextCodeFactory: public CipherFactory {
  public:
    TextCode* fromArgs(const clib::List<clib::String>& args) const override;
    TextCode* fromCin() const override;
    clib::String type() const override;
  };


}
