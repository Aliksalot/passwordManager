#pragma once

#include<fstream>
#include<iostream>

#include"./Password.h"
#include"../utils/array.h"
#include"../encrypt/Cypher.h"
#include"../encrypt/CipherConfig.h"
#include"../utils/Serializer.h"

namespace core{
  class PasswordFile{

  public:
    PasswordFile() = delete;
    PasswordFile(clib::String path);
    ~PasswordFile();

    clib::List<PasswordEntry> find(
      const clib::String& website = "",
      const clib::String& user = ""
    );
    PasswordFile& remove(const clib::String& website, const clib::String& user = "");
    PasswordFile& update(const PasswordEntry& p);

    //disable copy
    PasswordFile(const PasswordFile&) = delete;
    PasswordFile& operator=(const PasswordFile&) = delete;

    void createFile(const encrypt::CipherConfig& config);
    void load(const clib::String& filePassword);
    void save();

    bool hasUnsavedData() const;
  private:
    clib::List<PasswordEntry> passwords;
    clib::String path;
    encrypt::Cypher* cipher = nullptr;

    bool hasUnsaved = false;
  };

  inline PasswordFile::~PasswordFile() {
    delete cipher;
  }

  inline PasswordFile::PasswordFile(clib::String path): path(path) {};

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


  //TODO Not a good implementation
  inline PasswordFile& PasswordFile::update(const PasswordEntry& p) {
    if(long i = passwords.indexOf(p) >= 0) {
      passwords[i] = p;
    }else {
      passwords.add(p);
    }

    return *this;
  };

  inline void PasswordFile::createFile(
    const encrypt::CipherConfig& config
  ) {

    if(config.password.empty())
      throw std::runtime_error("Files must be password protected!");

    std::cout << config.password << " " << config.password.empty() << "\n";
    std::cout << utils::Serializer::cipherTypeToString(config.type) << "\n";

    auto newCipher = encrypt::CipherFactory::create(config);

    delete cipher;
    cipher = newCipher;

    std::ofstream file("./test");

    std::cout << path.raw() << std::endl;

    if(!file)
      throw std::runtime_error("Error when opening file to create");

    std::cout << "Created file!" << std::endl;
  }

  inline void PasswordFile::save() {

    if(!cipher)
      throw std::runtime_error("PasswordFile isn't intialized! ");

    std::ofstream file(path.raw());

    if(!file)
      throw std::runtime_error("Error when opening file to save");

    file << utils::Serializer::serializeHeader(cipher);
    file << "\n";

    clib::String encryptedPasswords = cipher->encrypt(
      utils::Serializer::serializePasswords(passwords)
    );

    file << encryptedPasswords;

    hasUnsaved = false;
  }
  inline void PasswordFile::load(const clib::String& filePassword) {
    //ima li cypher?
    if(cipher) {
      //veche sme go cheli

    }else {
      //za sefte go vijdame
      encrypt::CipherConfig conf = utils::Serializer::deserializeHeader("");
      cipher = encrypt::CipherFactory::create(encrypt::CipherConfig(conf, filePassword));
    }
  }

}
