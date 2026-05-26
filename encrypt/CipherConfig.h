#pragma once
#include"../utils/string.h"
#include"../utils/array.h"
#include"./CipherTypes.h"

namespace encrypt {
  struct CipherConfig {
    CipherType type;
    clib::String password;
    clib::List<clib::String> args;

    CipherConfig(CipherType type, const clib::String& password) 
      : password(password), type(type) { }
    CipherConfig(CipherType type, const clib::String& password, const clib::List<clib::String>& args) 
      : password(password), type(type), args(args) { }
    CipherConfig(CipherType type, const clib::List<clib::String>& args) 
      : args(args), type(type) { }
    CipherConfig(const CipherConfig& c, const clib::String& password)
      : args(c.args), type(c.type), password(password) {}
  };
}
