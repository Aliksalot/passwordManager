#pragma once
#include"../utils/mystring.h"
#include"./CipherTypeUtils.h"

namespace encrypt {
  class Cipher {
  public: 
    virtual clib::Text encrypt(const clib::Text& text) const = 0;
    virtual clib::Text decrypt(const clib::Text& text) const = 0;

    virtual clib::Text serialize() const = 0;
    virtual clib::Text type() const = 0;

    virtual ~Cipher() = default;
    virtual Cipher* clone() const = 0;
  };
}
