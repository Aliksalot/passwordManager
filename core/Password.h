#pragma once

#include"../utils/string.h"
#include"../encrypt/Cipher.h"
#include"../encrypt/CipherTypeUtils.h"

namespace core { 
  class PasswordEntry {
    public:
      PasswordEntry();
      ~PasswordEntry();
      PasswordEntry(
          const clib::String& website,
          const clib::String& username,
          const clib::String& passwordEncrypted,
          encrypt::Cipher* cipher
      );
      PasswordEntry(const PasswordEntry& p);
      PasswordEntry(PasswordEntry&& p);
      PasswordEntry& operator=(const PasswordEntry& p);
      PasswordEntry& operator=(PasswordEntry&& p);

      bool operator==(const PasswordEntry& p) const;
      clib::String getWebsite() const;
      clib::String getUsername() const;
      void setUsername(const clib::String& username);
      void setWebsite(const clib::String& website);
      void setPassword(const clib::String& passwordEncrypted);
      clib::String getPasswordEncrypted() const;
      const encrypt::Cipher* getCipher() const;
      clib::String serialize() const;

    private:
      clib::String website;
      clib::String username;
      clib::String passwordEncrypted;
      encrypt::Cipher* cipher;
  };

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

  clib::String PasswordEntry::getWebsite() const {
    return website;
  }
  clib::String PasswordEntry::getUsername() const {
    return username;
  }
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
        + "\t" + cipherTypeToString(cipher->type()) + "\t" + cipher->serialize();
  }
}
