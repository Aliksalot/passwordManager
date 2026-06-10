#include"PasswordView.h"

namespace core {
  clib::String PasswordView::getWebsite() const {
    return website;
  }
  clib::String PasswordView::getUsername() const {
    return username;
  }
  PasswordView::PasswordView(const clib::String& website, const clib::String& username)
    : website(website), username(username) { };

  clib::String PasswordView::serialize() const {
    return website + "\t" + username;
  }
}
