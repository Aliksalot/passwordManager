#pragma once

#include"../utils/string.h"

namespace core {
  struct PasswordView {
  protected:
    clib::String username;
    clib::String website;
  public:
    clib::String getWebsite() const;
    clib::String getUsername() const;
  };
}
