#pragma once

#include<iostream>

#include"./PasswordFile.h"
#include"./Password.h"
#include"../encrypt/CipherTypeUtils.h"
#include"../encrypt/ciphers/TestCypher.h"
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

      f.add("google.com", "u1", "password1");
      f.add("google.com", "u2", "password2");

      f.save("password");

      try{
        f.load("kur");
        std::cout << encrypt::cipherTypeToString(f.getDefaultCipher()->type()) << " " << f.getDefaultCipher()->serialize() << std::endl;
        auto pws = f.find();
        for(auto i = 0; i < pws.size(); i ++) {
          std::cout << pws[i].serialize() << std::endl;
          std::cout << "Decrypted: " << pws[i].getCipher()->decrypt(pws[i].getPasswordEncrypted()) << std::endl;
        }
      }catch(const utils::InvalidCipherTypeException& e) {
        std::cout << e.what() << std::endl;
      }catch(...) {
        std::cout << "Smth else" << std::endl;
      }
    };

  private:
    core::PasswordFile f = core::PasswordFile(clib::String("./test"));
  };
}
