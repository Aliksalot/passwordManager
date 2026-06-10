#include"PasswordView.h"

namespace core {
  clib::Text PasswordView::getWebsite() const {
    return website;
  }
  clib::Text PasswordView::getUsername() const {
    return username;
  }
  PasswordView::PasswordView(const clib::Text& website, const clib::Text& username)
    : website(website), username(username) { };

  clib::Text PasswordView::serialize() const {
    return website + "\t" + username;
  }
}
