#pragma once

#include"../utils/darray.h"
#include"../utils/mystring.h"
#include"../utils/exceptions.h"
#include"../math/SqMatrix.h"
#include"Cipher.h"

namespace gui {
  class Shell;
}

namespace encrypt {

  class CipherFactory {
  public:
    virtual Cipher* fromArgs(const clib::darray<clib::Text>& args) const = 0;
    virtual Cipher* fromShell(gui::Shell& shell) const = 0;
    virtual clib::Text type() const = 0;

    virtual ~CipherFactory() = default;
  };

}
