#pragma once

#include"../utils/array.h"
#include"../utils/string.h"
#include"../utils/exceptions.h"
#include"../math/SqMatrix.h"
#include"Cipher.h"

namespace gui {
  class Shell;
}

namespace encrypt {

  class CipherFactory {
  public:
    virtual Cipher* fromArgs(const clib::List<clib::String>& args) const = 0;
    virtual Cipher* fromShell(gui::Shell& shell) const = 0;
    virtual clib::String type() const = 0;

    virtual ~CipherFactory() = default;
  };

}
