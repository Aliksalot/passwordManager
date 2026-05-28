#pragma once
#include"../utils/string.h"
#include"../utils/array.h"
#include"./CipherTypeUtils.h"

namespace encrypt {
  struct CipherConfig {
    CipherType type;
    clib::List<clib::String> args;

    CipherConfig(CipherType type, const clib::List<clib::String>& args) : args(args), type(type) { }
  };
}
