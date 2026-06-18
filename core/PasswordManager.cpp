#include"PasswordManager.h"
#include"../utils/exceptions.h"

namespace core {

  void PasswordManager::checkFileOpenOrThrow() const {
    if(!hasOpenFile())
      throw utils::NoFileOpenError();
  }

  PasswordManager::PasswordManager() : currentFile(nullptr) { };

  PasswordManager::~PasswordManager() {
    delete currentFile;
  }

  bool PasswordManager::createFile(
    const clib::Text& fname,
    encrypt::Cipher* defaultCipher,
    const clib::Text& password
  ) {
    PasswordFile f(fname);
    
    try{
      f.createFile(defaultCipher, password);
    }catch(const utils::FileError& e) {
      return false;
    }
    return true;
  }

  void PasswordManager::openFile(const clib::Text& fname, const clib::Text& password) {
    
    try{
      if(currentFile != nullptr) {
        std::cout << "Opening a new file atop of another! Should be unreachable by users" << std::endl;
        delete currentFile;
      }
      currentFile = new PasswordFile(fname);
      currentFile->load(password);
    }catch(...) {
      delete currentFile;
      currentFile = nullptr;
      throw;
    }
  }

  clib::Text PasswordManager::getFilePath() const {
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
  }

  bool PasswordManager::isFileDirty() const {
    checkFileOpenOrThrow();

    return currentFile->hasUnsavedData();
  }

  void PasswordManager::saveFile(const clib::Text& password) {
    checkFileOpenOrThrow();

    currentFile->save(password);
  }

  clib::darray<const PasswordEntry*> PasswordManager::loadPassword(
    const clib::Text& website,
    const clib::Text& user
  ) const {
    checkFileOpenOrThrow();

    return currentFile->find(website, user);
  }

  void PasswordManager::updatePassword(
    const clib::Text& website,
    const clib::Text& user,
    const clib::Text& newPassword
  ) {
    checkFileOpenOrThrow();

    currentFile->update(website, user, newPassword);
  }

  bool PasswordManager::newPassword(
    const clib::Text& website,
    const clib::Text& user,
    const clib::Text& newPassword,
    encrypt::Cipher* cipher
  ) {
    checkFileOpenOrThrow();

    try{
      currentFile->add(website, user, newPassword, cipher);
    }catch(const utils::FileError& e) {
      std::cout << e.what() << std::endl;
      return false;
    }catch(const std::exception& e) {
      std::cout << e.what() << std::endl;
      return false;
    }

    return true;
  }


  void PasswordManager::deletePassword(
    const clib::Text& website,
    const clib::Text& user
  ) {
    checkFileOpenOrThrow();

    currentFile->remove(website, user);
  }
  
  clib::darray<const PasswordView*> PasswordManager::list() const {
    checkFileOpenOrThrow();

    auto plist = currentFile->find();
    clib::darray<const PasswordView*> viewList;
    for(auto& pEntry: plist) {
      viewList.add(pEntry);
    }

    return viewList;
  }

  PasswordManager::PasswordManager(PasswordManager&& pm) noexcept {
    currentFile = pm.currentFile;
    pm.currentFile = nullptr;
  }

  PasswordManager& PasswordManager::operator=(PasswordManager&& pm) noexcept {
    if(this == &pm) return *this;

    delete currentFile;

    currentFile = pm.currentFile;
    pm.currentFile = nullptr;

    return *this;
  }
}
