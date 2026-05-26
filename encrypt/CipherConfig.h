#pragma once
#include"../utils/string.h"
#include"../utils/array.h"
#include"./CipherType.h"

namespace encrypt {
  struct CipherConfig {
    encrypt::CipherType t;

    clib::String password;


    clib::List<clib::String>& args;

  }
}
