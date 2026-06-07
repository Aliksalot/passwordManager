#include"PasswordManager.h"
#include"../utils/exceptions.h"

namespace core {

  void PasswordManager::checkFileOpenOrThrow() const {
    if(currentFile == nullptr)
      throw utils::NoFileOpenError();
  }

  PasswordManager::PasswordManager() : currentFile(nullptr) { };

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
    checkFileOpenOrThrow();
    
    try{
      currentFile = new PasswordFile(fname);
      currentFile->load(password);
    }catch(...) {
      delete currentFile;
      throw;
    }
  }
  

  bool PasswordManager::closeFile() {
    checkFileOpenOrThrow();

    if(currentFile->hasUnsavedData()) {
      return false;
    }

    delete currentFile;
    currentFile = nullptr;

    return true;
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
  }

  PasswordManager& PasswordManager::operator=(PasswordManager&& pm) noexcept {
    if(this == &pm) return *this;

    delete currentFile;

    currentFile = pm.currentFile;
    pm.currentFile = nullptr;

    return *this;
  }
}
