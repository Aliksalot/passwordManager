#pragma once

#include"../utils/mystring.h"

namespace core {
  class PasswordView {
  protected:
    clib::Text website;
    clib::Text username;
    
    PasswordView(const clib::Text& website, const clib::Text& username);
    PasswordView() = default;
  public:
    clib::Text getWebsite() const;
    clib::Text getUsername() const;

    virtual clib::Text serialize() const;
  };
}
