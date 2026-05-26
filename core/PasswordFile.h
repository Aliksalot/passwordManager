#pragma once

#include"./Password.h"
#include"../utils/array.h"
#include"../encrypt/Cypher.h"
#include"../utils/Serializer.h"

namespace core{
  class PasswordFile{

  public:
    PasswordFile();
    PasswordFile(clib::String path);

    clib::List<PasswordEntry> find(
      const clib::String& website = "",
      const clib::String& user = ""
    );
    PasswordFile& remove(const clib::String& website, const clib::String& user = "");
    PasswordFile& update(const PasswordEntry& p);

    //disable copy
    PasswordFile(const PasswordFile&) = delete;
    PasswordFile& operator=(const PasswordFile&) = delete;

    void createFile(
      encrypt::CipherType t,
      const clib::String& filePassword,
      const clib::List<clib::String>& conf = clib::List<clib::String>()
    );
    void load(const clib::String& filePassword);
    void save();

    bool hasUnsavedData() const;
  private:
    clib::List<PasswordEntry> passwords;
    clib::String path;
    encrypt::Cypher* cipher;

    bool hasUnsaved = false;
  };


  inline PasswordFile::PasswordFile() {};

  inline PasswordFile::PasswordFile(encrypt::Cypher* c): cipher(c) {};

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

  inline void PasswordFile::save() const {

    clib::String content;
    content += serializeHeader(cipher) + "\n";
    auto encryptedPasswords = cipher->encrypt(
      utils::Serializer::serializePasswords(passwords);
    );
    content += encryptedPasswords;

  }
  inline void PasswordFile::load(const clib::String& filePassword) const {
    //get enc type
    
  }

}
