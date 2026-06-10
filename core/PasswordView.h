#pragma once

#include"../utils/string.h"

namespace core {
  struct PasswordView {
  protected:
    clib::String website;
    clib::String username;
    
    PasswordView(const clib::String& website, const clib::String& username);
    PasswordView() = default;
  public:
    clib::String getWebsite() const;
    clib::String getUsername() const;

    virtual clib::String serialize() const;
  };
}
