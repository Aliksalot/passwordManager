#include"./PasswordFile.h"

namespace core {
  PasswordFile::~PasswordFile() {
    delete defaultCipher;
  }

  PasswordFile::PasswordFile(clib::Text path): path(path) {};

  const encrypt::Cipher* PasswordFile::getDefaultCipher() const {
    return defaultCipher;
  }

  clib::Text PasswordFile::getPath() const {
    return path;
  }

  clib::darray<const PasswordEntry*> PasswordFile::find(
      const clib::Text& website,
      const clib::Text& user
  ) const {
    clib::darray<const PasswordEntry*> r;
    for(std::size_t i = 0; i < passwords.size(); i ++) {
      bool websiteMatch = website.empty() || website == passwords[i].getWebsite();
      bool userMatch = user.empty() || user == passwords[i].getUsername();
      if(websiteMatch && userMatch) 
        r.add(&passwords[i]);
    }
    return r;
  }

  PasswordFile& PasswordFile::remove(
      const clib::Text& website,
      const clib::Text& user
  ) {
    for(std::size_t i = 0; i < passwords.size(); ) {
      bool websiteMatch = website.empty() || website == passwords[i].getWebsite();
      bool userMatch = user.empty() || user == passwords[i].getUsername();
      if(websiteMatch && userMatch) {
        passwords.remove(i);
        hasUnsaved = true;
      }else {
        i ++;
      }
    }
    return *this;
  };


  PasswordFile& PasswordFile::add(
    const clib::Text& website,
    const clib::Text& username,
    const clib::Text& password,
    encrypt::Cipher* cipher
  ) {
    if(defaultCipher == nullptr)
      throw utils::PasswordModifyError(
        "Error when creating new user - PasswordFile missing defaultCipher (Not initialized)"
      );

    for(std::size_t i = 0; i < passwords.size(); i ++) {
      PasswordEntry& p = passwords[i];
      if(p.getWebsite() == website && p.getUsername() == username) {
        delete cipher;
        throw utils::PasswordModifyError(
          "Error when creating new user - Person with such credentials already exists!"
        );
      }
    }

    auto passCipher = cipher == nullptr ? defaultCipher->clone() : cipher;

    try{
      passwords.add(PasswordEntry(
        website, username,
        passCipher->encrypt(password),
        passCipher
      ));
    }catch(...) {
      delete passCipher;
      throw;
    }

    hasUnsaved = true;

    return *this;
  }
  PasswordFile& PasswordFile::update(
    const clib::Text& website,
    const clib::Text& username,
    const clib::Text& password
  ) {

    bool foundMatch = false;
    for(std::size_t i = 0; i < passwords.size(); i ++) {
      PasswordEntry& p = passwords[i];
      if(p.getWebsite() == website && p.getUsername() == username) {
        if(
          p.getCipher()->decrypt(p.getPasswordEncrypted()) == password
        ) {
          throw utils::PasswordModifyError("Error when updating password - New password matches old one!");
        }
        p.setPassword(
          p.getCipher()->encrypt(password)
        );
        hasUnsaved = true;
        foundMatch = true;
      }
    }
    if(!foundMatch) {
      throw utils::PasswordModifyError("Error when updating password - Couldn't find a password entry to update!");
    }
    return *this;
  };

  void PasswordFile::createFile(
    encrypt::Cipher* cipher,
    const clib::Text& password
  ) {

    std::ifstream fileInput(path.raw(), std::ios::binary);

    if(fileInput)
      throw utils::FileError("File already exists!");

    delete defaultCipher;
    defaultCipher = cipher;

    std::ofstream file(path.raw(), std::ios::binary);

    if(!file)
      throw utils::FileError("Error when opening file to create");

    save(password);
  }

  void PasswordFile::save(const clib::Text& password) {

    if(!defaultCipher)
      throw utils::FileError("Error when saving file - defaultCipher is missing \
          (PasswordFile not initialized)");

    std::ofstream file(path.raw(), std::ios::binary);

    if(!file)
      throw utils::FileError("Error when opening file for saving. Possibly missing");

    clib::Text content;

    content += utils::Serializer::serializeCipher(defaultCipher) + "\n";
    content += utils::Serializer::serializePasswords(passwords);

    clib::Text encryptedPasswords = utils::encryptFile(content, password);
    file.write(encryptedPasswords.raw(), encryptedPasswords.size());

    hasUnsaved = false;
  }

  void PasswordFile::load(const clib::Text& password) {
    std::ifstream file(path.raw(), std::ios::binary);

    if(!file)
      throw utils::FileError("Possibly missing file");

    clib::Text content;
    char c;

    while(file.get(c)) {
        content += c;
    }

    content = utils::decryptFile(content, password);

    clib::darray<clib::Text> lines = content.split('\n');

    if(lines.size() == 0)
      throw utils::EncryptionError();

    auto newCipher = utils::Serializer::deserializeCipher(lines[0]);
    delete defaultCipher;
    defaultCipher = newCipher;
    lines.remove(0);
    passwords = utils::Serializer::deserializePasswords(lines);
  }

  bool PasswordFile::hasUnsavedData() const {
    return hasUnsaved;
  }
}
