#pragma once

#include"../utils/mystring.h"
#include"./PasswordView.h"
#include"../encrypt/Cipher.h"
#include"../encrypt/CipherTypeUtils.h"

namespace core { 
  class PasswordEntry: public PasswordView {
    public:
      PasswordEntry();
      ~PasswordEntry();
      PasswordEntry(
          const clib::Text& website,
          const clib::Text& username,
          const clib::Text& passwordEncrypted,
          encrypt::Cipher* cipher
      );
      PasswordEntry(const PasswordEntry& p);
      PasswordEntry(PasswordEntry&& p);
      PasswordEntry& operator=(const PasswordEntry& p);
      PasswordEntry& operator=(PasswordEntry&& p);

      bool operator==(const PasswordEntry& p) const;
      void setUsername(const clib::Text& username);
      void setWebsite(const clib::Text& website);
      void setPassword(const clib::Text& passwordEncrypted);
      clib::Text getPasswordEncrypted() const;
      const encrypt::Cipher* getCipher() const;
      clib::Text serialize() const override;
    private:
      clib::Text passwordEncrypted;
      encrypt::Cipher* cipher;
  };

}
