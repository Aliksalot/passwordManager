#pragma once
#include"../utils/string.h"
#include"./CipherTypeUtils.h"

namespace encrypt {
  class Cipher {
  public: 
    virtual clib::String encrypt(const clib::String& text) const = 0;
    virtual clib::String decrypt(const clib::String& text) const = 0;

    virtual clib::String serialize() const = 0;
    virtual clib::String type() const = 0;

    virtual ~Cipher() = default;
    virtual Cipher* clone() const = 0;
  };
}
