#pragma once

#include<fstream>
#include<iostream>

#include"./Password.h"
#include"../utils/array.h"
#include"../encrypt/Cipher.h"
#include"../utils/FileEncrypt.h"
#include"../utils/Serializer.h"

namespace core{
  class PasswordFile{

  public:
    PasswordFile() = delete;
    PasswordFile(clib::String path);
    ~PasswordFile();

    const encrypt::Cipher* getDefaultCipher() const;
    clib::List<const PasswordEntry*> find(
      const clib::String& website = "",
      const clib::String& user = ""
    ) const;
    PasswordFile& remove(const clib::String& website, const clib::String& user = "");
    PasswordFile& update(
      const clib::String& website,
      const clib::String& username,
      const clib::String& password
    );
    PasswordFile& add(
      const clib::String& website,
      const clib::String& username,
      const clib::String& password,
      encrypt::Cipher* cipher = nullptr
    );

    //disable copy
    PasswordFile(const PasswordFile&) = delete;
    PasswordFile& operator=(const PasswordFile&) = delete;

    //disable move
    PasswordFile(PasswordFile&&) = delete;
    PasswordFile& operator=(PasswordFile&&) = delete;

    void createFile(encrypt::Cipher* cipher, const clib::String& password);
    void load(const clib::String& password);
    void save(const clib::String& password);

    bool hasUnsavedData() const;
  private:
    clib::List<PasswordEntry> passwords;
    clib::String path;
    encrypt::Cipher* defaultCipher = nullptr;

    bool hasUnsaved = false;
  };
}
