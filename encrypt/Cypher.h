#pragma once
#include"../utils/string.h"
#include"./CipherTypes.h"

namespace encrypt {
  class Cypher {
  public: 
    virtual clib::String encrypt(const clib::String& text) const = 0;
    virtual clib::String decrypt(const clib::String& text) const = 0;

    virtual clib::String serialize() const = 0;
    virtual CipherType type() const = 0;

    virtual ~Cypher() = default;
    virtual Cypher* clone() const = 0;
  };
}
