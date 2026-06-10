#include"PasswordManager.h"
#include"../utils/exceptions.h"

namespace core {

  void PasswordManager::checkFileOpenOrThrow() const {
    if(!hasOpenFile())
      throw utils::NoFileOpenError();
  }

  PasswordManager::PasswordManager() : currentFile(nullptr), password("") { };

  PasswordManager::~PasswordManager() {
    delete currentFile;
  }

  void PasswordManager::createFile(
    const clib::String& fname,
    encrypt::Cipher* defaultCipher,
    const clib::String& password
  ) {
    PasswordFile f(fname);
    
    f.createFile(defaultCipher, password);
  }

  void PasswordManager::openFile(const clib::String& fname, const clib::String& password) {
    
    try{
      if(currentFile != nullptr) {
        std::cout << "Opening a new file atop of another! Should be unreachable by users" << std::endl;
        delete currentFile;
      }
      currentFile = new PasswordFile(fname);
      currentFile->load(password);
      this->password = password;
    }catch(...) {
      delete currentFile;
      currentFile = nullptr;
      throw;
    }
  }

  clib::String PasswordManager::getFilePath() const {
    checkFileOpenOrThrow();

    return currentFile->getPath();
  }
  bool PasswordManager::hasOpenFile() const {
    return currentFile != nullptr;
  }

  void PasswordManager::closeFile() {
    checkFileOpenOrThrow();

    delete currentFile;
    currentFile = nullptr;

    password = "";
  }

  bool PasswordManager::isFileDirty() {
    checkFileOpenOrThrow();

    return currentFile->hasUnsavedData();
  }

  void PasswordManager::saveFile() {
    checkFileOpenOrThrow();

    currentFile->save(password);
  }

  clib::List<const PasswordEntry*> PasswordManager::loadPassword(
    const clib::String& website,
    const clib::String& user
  ) const {
    checkFileOpenOrThrow();

    return currentFile->find(website, user);
  }

  void PasswordManager::updatePassword(
    const clib::String& website,
    const clib::String& user,
    const clib::String& newPassword
  ) {
    checkFileOpenOrThrow();

    currentFile->update(website, user, newPassword);
  }

  bool PasswordManager::newPassword(
    const clib::String& website,
    const clib::String& user,
    const clib::String& newPassword,
    encrypt::Cipher* cipher
  ) {
    checkFileOpenOrThrow();

    try{
      currentFile->add(website, user, newPassword, cipher);
    }catch(const utils::FileError& e) {
      std::cout << e.what() << std::endl;
      return false;
    }catch(...) {
      return false;
    }

    return true;
  }


  void PasswordManager::deletePassword(
    const clib::String& website,
    const clib::String& user
  ) {
    checkFileOpenOrThrow();

    currentFile->remove(website, user);
  }
  
  clib::List<const PasswordView*> PasswordManager::list() const {
    checkFileOpenOrThrow();

    auto plist = currentFile->find();
    clib::List<const PasswordView*> viewList;
    for(auto& pEntry: plist) {
      viewList.add(pEntry);
    }

    return viewList;
  }

  PasswordManager::PasswordManager(PasswordManager&& pm) noexcept {
    currentFile = pm.currentFile;
    pm.currentFile = nullptr;

    password = pm.password;
    pm.password = "";
  }

  PasswordManager& PasswordManager::operator=(PasswordManager&& pm) noexcept {
    if(this == &pm) return *this;

    delete currentFile;

    currentFile = pm.currentFile;
    pm.currentFile = nullptr;

    password = pm.password;
    pm.password = "";

    return *this;
  }
}
