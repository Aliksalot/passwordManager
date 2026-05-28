#pragma once

#include<iostream>

#include"./PasswordFile.h"
#include"./Password.h"
#include"../encrypt/CipherTypeUtils.h"
#include"../encrypt/ciphers/TestCipher.h"
#include"../encrypt/ciphers/TextCode.h"
#include"../encrypt/ciphers/CaesarCipher.h"
#include<iostream>

namespace core{
  
  class PasswordManager{

  public:
    PasswordManager() {

      f.createFile(
        new encrypt::CaesarCipher(22),
        "password"
      );

      f.add("google.com", "u1", "password1", new encrypt::TestCipher("Hello"));
      f.add("google.com", "u2", "password2", new encrypt::TextCode("./data/encryption"));
      f.add("google.com", "u3", "password3");

      f.save("password");

      try{
        f.load("password");
        std::cout << encrypt::cipherTypeToString(f.getDefaultCipher()->type()) << " " << f.getDefaultCipher()->serialize() << std::endl;
        auto pws = f.find();
        for(auto& p: pws) {
          std::cout << p.serialize() << std::endl;
          std::cout << "Decrypted: " << p.getCipher()->decrypt(p.getPasswordEncrypted()) << std::endl;
        }
      }catch(const utils::EncryptionError& e) {
        std::cout << e.what() << std::endl;
      }catch(const utils::FileError& e) {
        std::cout << e.what() << std::endl;
      }catch(...) {
        std::cout << "Some unknown error happened" << std::endl;
      }
    };

  private:
    core::PasswordFile f = core::PasswordFile(clib::String("./data/test1"));
  };
}
