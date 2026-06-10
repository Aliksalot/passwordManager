#pragma once

#include"../Cipher.h"
#include<fstream>
#include"../../utils/mystring.h"
#include"../../utils/darray.h"
#include"../../utils/exceptions.h"

#include"../CipherFactory.h"

namespace encrypt {

  struct CharMap {
    signed data[256];
  };

  class TextCode: public Cipher{
  private:

    clib::Text path;
    
    CharMap buildLetterToIndex() const;
    
    clib::Text readCipherFile() const;
  public:

    TextCode() = delete;
    TextCode(clib::Text path);
    TextCode(const char* path);

    clib::Text encrypt(const clib::Text& text) const override;
    clib::Text decrypt(const clib::Text& text) const override;

    clib::Text serialize() const override;
    clib::Text type() const override;

    TextCode* clone() const override;
  };

  class TextCodeFactory: public CipherFactory {
  public:
    TextCode* fromArgs(const clib::darray<clib::Text>& args) const override;
    TextCode* fromShell(gui::Shell& shell) const override;
    clib::Text type() const override;
  };


}
