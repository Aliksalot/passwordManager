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

  class PasswordManager {
  private:
    PasswordFile* currentFile = nullptr;

    void checkFileOpenOrThrow() const;
  public:
    PasswordManager();

    bool createFile(
      const clib::Text& fname,
      encrypt::Cipher* defaultCipher,
      const clib::Text& password
    );

    void openFile(const clib::Text& fname, const clib::Text& password);

    void closeFile();

    bool isFileDirty() const;
    
    clib::darray<const PasswordEntry*> loadPassword(
      const clib::Text& website,
      const clib::Text& user = ""
    ) const;

    void updatePassword(
      const clib::Text& website,
      const clib::Text& user,
      const clib::Text& newPassword
    );

    bool newPassword(
      const clib::Text& website,
      const clib::Text& user,
      const clib::Text& newPassword,
      encrypt::Cipher* cipher = nullptr
    );

    void deletePassword(const clib::Text& website, const clib::Text& user = "");

    bool hasOpenFile() const;
  
    clib::Text getFilePath() const;
    
    clib::darray<const PasswordView*> list() const;

    void saveFile(const clib::Text& password);

    ~PasswordManager();

    //delete copy - nonsensical, since PasswordFile isn't copyable either
    PasswordManager(const PasswordManager&) = delete;
    PasswordManager& operator=(const PasswordManager&) = delete;

    PasswordManager(PasswordManager&& pm) noexcept;
    PasswordManager& operator=(PasswordManager&& pm) noexcept;
  };
}
