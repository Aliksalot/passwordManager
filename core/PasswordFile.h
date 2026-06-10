#pragma once

#include<fstream>
#include<iostream>

#include"./Password.h"
#include"../utils/darray.h"
#include"../encrypt/Cipher.h"
#include"../utils/FileEncrypt.h"
#include"../utils/Serializer.h"

namespace core{
  class PasswordFile{

  public:
    PasswordFile() = delete;
    PasswordFile(clib::Text path);
    ~PasswordFile();

    const encrypt::Cipher* getDefaultCipher() const;
    clib::Text getPath() const;
    clib::darray<const PasswordEntry*> find(
      const clib::Text& website = "",
      const clib::Text& user = ""
    ) const;
    PasswordFile& remove(const clib::Text& website, const clib::Text& user = "");
    PasswordFile& update(
      const clib::Text& website,
      const clib::Text& username,
      const clib::Text& password
    );
    PasswordFile& add(
      const clib::Text& website,
      const clib::Text& username,
      const clib::Text& password,
      encrypt::Cipher* cipher = nullptr
    );

    //disable copy
    PasswordFile(const PasswordFile&) = delete;
    PasswordFile& operator=(const PasswordFile&) = delete;

    //disable move
    PasswordFile(PasswordFile&&) = delete;
    PasswordFile& operator=(PasswordFile&&) = delete;

    void createFile(encrypt::Cipher* cipher, const clib::Text& password);
    void load(const clib::Text& password);
    void save(const clib::Text& password);

    bool hasUnsavedData() const;
  private:
    clib::darray<PasswordEntry> passwords;
    clib::Text path;
    encrypt::Cipher* defaultCipher = nullptr;

    bool hasUnsaved = false;
  };
}
