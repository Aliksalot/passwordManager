#pragma once

#include<fstream>
#include<iostream>

#include"./Password.h"
#include"../utils/array.h"
#include"../encrypt/Cypher.h"
#include"../utils/FileEncrypt.h"
#include"../utils/Serializer.h"

namespace core{
  class PasswordFile{

  public:
    PasswordFile() = delete;
    PasswordFile(clib::String path);
    ~PasswordFile();

    const encrypt::Cypher* getDefaultCipher() const;
    clib::List<PasswordEntry> find(
      const clib::String& website = "",
      const clib::String& user = ""
    );
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
      encrypt::Cypher* cipher = nullptr
    );

    //disable copy
    PasswordFile(const PasswordFile&) = delete;
    PasswordFile& operator=(const PasswordFile&) = delete;

    void createFile(encrypt::Cypher* cipher, const clib::String& password);
    void load(const clib::String& password);
    void save(const clib::String& password);

    bool hasUnsavedData() const;
  private:
    clib::List<PasswordEntry> passwords;
    clib::String path;
    encrypt::Cypher* defaultCipher = nullptr;

    bool hasUnsaved = false;
  };

  inline PasswordFile::~PasswordFile() {
    delete defaultCipher;
  }

  inline PasswordFile::PasswordFile(clib::String path): path(path) {};

  inline const encrypt::Cypher* PasswordFile::getDefaultCipher() const {
    return defaultCipher;
  }

  inline clib::List<PasswordEntry> PasswordFile::find(
      const clib::String& website,
      const clib::String& user
  ) {
    clib::List<PasswordEntry> r;
    for(std::size_t i = 0; i < passwords.size(); i ++) {
      bool websiteMatch = website.empty() || website == passwords[i].getWebsite();
      bool userMatch = user.empty() || user == passwords[i].getUsername();
      if(websiteMatch && userMatch) r.add(passwords[i]);
    }
    return r;
  }

  inline PasswordFile& PasswordFile::remove(
      const clib::String& website,
      const clib::String& user
  ) {
    for(std::size_t i = 0; i < passwords.size(); ) {
      bool websiteMatch = website.empty() || website == passwords[i].getWebsite();
      bool userMatch = user.empty() || user == passwords[i].getUsername();
      if(websiteMatch && userMatch) {
        passwords.remove(i);
      }else {
        i ++;
      }
    }
    return *this;
  };


  inline PasswordFile& PasswordFile::add(
    const clib::String& website,
    const clib::String& username,
    const clib::String& password,
    encrypt::Cypher* cipher
  ) {
    if(defaultCipher == nullptr)
      throw new std::runtime_error(
        "Error when creatign new user - PasswordFile missing defaultCipher (Not initialized)"
      );

    for(std::size_t i = 0; i < passwords.size(); i ++) {
      PasswordEntry& p = passwords[i];
      if(p.getWebsite() == website && p.getUsername() == username)
        throw new std::runtime_error(
          "Error when creating new user - Person with such credentials already exists!"
        );
    }

    auto passCipher = cipher == nullptr ? defaultCipher : cipher;

    passwords.add(PasswordEntry(
      website, username,
      passCipher->encrypt(password),
      passCipher->clone()
    ));

    return *this;
  }
  inline PasswordFile& PasswordFile::update(
    const clib::String& website,
    const clib::String& username,
    const clib::String& password
  ) {
    for(std::size_t i = 0; i < passwords.size(); i ++) {
      PasswordEntry& p = passwords[i];
      if(p.getWebsite() == website && p.getUsername() == username) {
        p.setPassword(
          p.getCipher()->encrypt(password)
        );
      }
    }
    return *this;
  };

  inline void PasswordFile::createFile(
    encrypt::Cypher* cipher,
    const clib::String& password
  ) {

    delete defaultCipher;
    defaultCipher = cipher;

    std::ofstream file(path.raw(), std::ios::binary);

    if(!file)
      throw std::runtime_error("Error when opening file to create");

    save(password);
  }

  inline void PasswordFile::save(const clib::String& password) {

    if(!defaultCipher)
      throw std::runtime_error("PasswordFile isn't intialized! ");

    std::ofstream file(path.raw(), std::ios::binary);

    if(!file)
      throw std::runtime_error("Error when opening file to save");

    clib::String content;

    content += utils::Serializer::serializeCipher(defaultCipher) + "\n";
    content += utils::Serializer::serializePasswords(passwords);

    clib::String encryptedPasswords = utils::FileEncrypt::encrypt(content, password);
    file.write(encryptedPasswords.raw(), encryptedPasswords.size());

    hasUnsaved = false;
  }

  inline void PasswordFile::load(const clib::String& password) {
    std::ifstream file(path.raw(), std::ios::binary);

    clib::String content;
    char c;

    while(file.get(c)) {
        content += c;
    }

    content = utils::FileEncrypt::decrypt(content, password);

    clib::List<clib::String> lines = content.split('\n');


    if(lines.size() == 0)
      throw std::runtime_error("On load: file possibly corrupted");

    delete defaultCipher;
    defaultCipher = utils::Serializer::deserializeCipher(lines[0]);
    lines.remove(0);
    passwords = utils::Serializer::deserializePasswords(lines);
  }
}
