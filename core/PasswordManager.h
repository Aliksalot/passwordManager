#pragma once

#include<iostream>

#include"./PasswordFile.h"
#include"./Password.h"
#include"../encrypt/CipherTypeUtils.h"
#include"../encrypt/ciphers/TestCipher.h"
#include"../encrypt/ciphers/TextCode.h"
#include"../encrypt/ciphers/CaesarCipher.h"
#include"../encrypt/ciphers/HillCipher.h"
#include<iostream>

namespace core{
  
  /*Called needs to handle utils::NoFileOpenError by convention*/
  class PasswordManager {
  private:
    PasswordFile* currentFile = nullptr;

    void checkFileOpenOrThrow() const;
  public:
    PasswordManager();

    void createFile(
      const clib::String& fname,
      encrypt::Cipher* defaultCipher,
      const clib::String& password
    );

    void openFile(const clib::String& fname, const clib::String& password);

    bool closeFile();
    
    clib::List<const PasswordEntry*> loadPassword(
      const clib::String& website,
      const clib::String& user = ""
    ) const;

    void updatePassword(
      const clib::String& website,
      const clib::String& user,
      const clib::String& newPassword
    );

    void deletePassword(const clib::String& website, const clib::String& user = "");
    
    clib::List<const PasswordView*> list() const;

    ~PasswordManager();

    //delete copy - nonsensical, since PasswordFile isn't copyable either
    PasswordManager(const PasswordManager&) = delete;
    PasswordManager& operator=(const PasswordManager&) = delete;

    PasswordManager(PasswordManager&& pm) noexcept;
    PasswordManager& operator=(PasswordManager&& pm) noexcept;
  };
}
