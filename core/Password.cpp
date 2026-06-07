#include"Password.h"

namespace core {
  PasswordEntry::PasswordEntry(): cipher(nullptr) {};

  PasswordEntry::~PasswordEntry() {
    delete cipher;
  }

  PasswordEntry::PasswordEntry(
      const clib::String& website,
      const clib::String& username,
      const clib::String& passwordEncrypted,
      encrypt::Cipher* cipher
  ): website(website),username(username), passwordEncrypted(passwordEncrypted),cipher(cipher) {};

  void PasswordEntry::setUsername(const clib::String& username) {
    this->username = username;
  }
  void PasswordEntry::setWebsite(const clib::String& website) {
    this->website = website;
  }
  void PasswordEntry::setPassword(const clib::String& passwordEncrypted) {
    this->passwordEncrypted = passwordEncrypted;
  }
  clib::String PasswordEntry::getPasswordEncrypted() const {
    return passwordEncrypted;
  }
  const encrypt::Cipher* PasswordEntry::getCipher() const {
    return cipher;
  }
  
  PasswordEntry::PasswordEntry(const PasswordEntry& p):
    website(p.website),
    username(p.username),
    passwordEncrypted(p.passwordEncrypted),
    cipher(p.cipher->clone())
  { }
  PasswordEntry::PasswordEntry(PasswordEntry&& p):
    website(p.website),
    username(p.username),
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

  clib::String PasswordEntry::serialize() const {
    return 
      website + "\t" + username + "\t" + passwordEncrypted
        + "\t" + cipher->type() + "\t" + cipher->serialize();
  }
}
