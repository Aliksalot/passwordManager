#include"Password.h"

namespace core {
  PasswordEntry::PasswordEntry(): cipher(nullptr) {};

  PasswordEntry::~PasswordEntry() {
    delete cipher;
  }

  PasswordEntry::PasswordEntry(
      const clib::Text& website,
      const clib::Text& username,
      const clib::Text& passwordEncrypted,
      encrypt::Cipher* cipher
  ): PasswordView(website, username), passwordEncrypted(passwordEncrypted),cipher(cipher) {};

  void PasswordEntry::setUsername(const clib::Text& username) {
    this->username = username;
  }
  void PasswordEntry::setWebsite(const clib::Text& website) {
    this->website = website;
  }
  void PasswordEntry::setPassword(const clib::Text& passwordEncrypted) {
    this->passwordEncrypted = passwordEncrypted;
  }
  clib::Text PasswordEntry::getPasswordEncrypted() const {
    return passwordEncrypted;
  }
  const encrypt::Cipher* PasswordEntry::getCipher() const {
    return cipher;
  }
  
  PasswordEntry::PasswordEntry(const PasswordEntry& p):
    PasswordView(p.website, p.username),
    passwordEncrypted(p.passwordEncrypted),
    cipher(p.cipher->clone())
  { }
  PasswordEntry::PasswordEntry(PasswordEntry&& p):
    PasswordView(p.website, p.username),
    passwordEncrypted(p.passwordEncrypted)
  { 
    p.website = "";
    p.username = "";
    p.passwordEncrypted = "";

    cipher = p.cipher;
    p.cipher = nullptr;
  }
  PasswordEntry& PasswordEntry::operator=(const PasswordEntry& p) {
    if(this == &p) return *this;

    website = p.website;
    username = p.username;
    passwordEncrypted = p.passwordEncrypted;

    delete cipher;
    cipher = p.cipher->clone();

    return *this;
  }
  PasswordEntry& PasswordEntry::operator=(PasswordEntry&& p) {
    if(this == &p) return *this;

    website = p.website;
    username = p.username;
    passwordEncrypted = p.passwordEncrypted;

    p.website = "";
    p.username = "";
    p.passwordEncrypted = "";

    delete cipher;
    cipher = p.cipher;
    p.cipher = nullptr;

    return *this;
  }
  bool PasswordEntry::operator==(const PasswordEntry& p) const {
    return website == p.website && username == p.username;
  };

  clib::Text PasswordEntry::serialize() const {
    return 
      website + "\t" + username + "\t" + passwordEncrypted
        + "\t" + cipher->type() + "\t" + cipher->serialize();
  }
}
