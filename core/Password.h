#pragma once

#include"../utils/string.h"
#include"./PasswordView.h"
#include"../encrypt/Cipher.h"
#include"../encrypt/CipherTypeUtils.h"

namespace core { 
  class PasswordEntry: public PasswordView {
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
      void setUsername(const clib::String& username);
      void setWebsite(const clib::String& website);
      void setPassword(const clib::String& passwordEncrypted);
      clib::String getPasswordEncrypted() const;
      const encrypt::Cipher* getCipher() const;
      clib::String serialize() const override;
    private:
      clib::String passwordEncrypted;
      encrypt::Cipher* cipher;
  };

}
