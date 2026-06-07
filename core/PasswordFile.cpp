#include"./PasswordFile.h"

namespace core {
  PasswordFile::~PasswordFile() {
    delete defaultCipher;
  }

  PasswordFile::PasswordFile(clib::String path): path(path) {};

  const encrypt::Cipher* PasswordFile::getDefaultCipher() const {
    return defaultCipher;
  }

  clib::List<PasswordEntry> PasswordFile::find(
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

  PasswordFile& PasswordFile::remove(
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


  PasswordFile& PasswordFile::add(
    const clib::String& website,
    const clib::String& username,
    const clib::String& password,
    encrypt::Cipher* cipher
  ) {
    if(defaultCipher == nullptr)
      throw std::runtime_error(
        "Error when creating new user - PasswordFile missing defaultCipher (Not initialized)"
      );

    for(std::size_t i = 0; i < passwords.size(); i ++) {
      PasswordEntry& p = passwords[i];
      if(p.getWebsite() == website && p.getUsername() == username)
        throw std::runtime_error(
          "Error when creating new user - Person with such credentials already exists!"
        );
    }

    auto passCipher = cipher == nullptr ? defaultCipher->clone() : cipher;

    passwords.add(PasswordEntry(
      website, username,
      passCipher->encrypt(password),
      passCipher
    ));

    return *this;
  }
  PasswordFile& PasswordFile::update(
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

  void PasswordFile::createFile(
    encrypt::Cipher* cipher,
    const clib::String& password
  ) {

    delete defaultCipher;
    defaultCipher = cipher;

    std::ofstream file(path.raw(), std::ios::binary);

    if(!file)
      throw utils::FileError("Error when opening file to create");

    save(password);
  }

  void PasswordFile::save(const clib::String& password) {

    if(!defaultCipher)
      throw std::runtime_error("Error when saving file - defaultCipher is missing \
          (PasswordFile not initialized)");

    std::ofstream file(path.raw(), std::ios::binary);

    if(!file)
      throw utils::FileError("Error when opening file for saving. Possibly missing");

    clib::String content;

    content += utils::Serializer::serializeCipher(defaultCipher) + "\n";
    content += utils::Serializer::serializePasswords(passwords);

    clib::String encryptedPasswords = utils::FileEncrypt::encrypt(content, password);
    file.write(encryptedPasswords.raw(), encryptedPasswords.size());

    hasUnsaved = false;
  }

  void PasswordFile::load(const clib::String& password) {
    std::ifstream file(path.raw(), std::ios::binary);

    if(!file)
      throw utils::FileError("Possibly missing file");

    clib::String content;
    char c;

    while(file.get(c)) {
        content += c;
    }

    content = utils::FileEncrypt::decrypt(content, password);

    clib::List<clib::String> lines = content.split('\n');

    for(auto& line: lines) {
      std::cout << line << std::endl;
    }

    if(lines.size() == 0)
      throw utils::EncryptionError();

    auto newCipher = utils::Serializer::deserializeCipher(lines[0]);
    delete defaultCipher;
    defaultCipher = newCipher;
    lines.remove(0);
    passwords = utils::Serializer::deserializePasswords(lines);
  }
}
