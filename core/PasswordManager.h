#pragma once

#include<iostream>

#include"./PasswordFile.h"
#include"./Password.h"
#include"../encrypt/CipherTypeUtils.h"
#include"../encrypt/TestCypher.h"
#include<iostream>

namespace core{
  
  class PasswordManager{

  public:
    PasswordManager() {
      f.createFile(
        new encrypt::TestCypher("Helloa"),
        "password"
      );

      f.add("google.com", "u1", "password1");
      f.add("google.com", "u2", "password2");

      f.save("password");

      try{
        f.load("password");
        std::cout << encrypt::cipherTypeToString(f.getDefaultCipher()->type()) << " " << f.getDefaultCipher()->serialize() << std::endl;
        auto pws = f.find();
        for(auto i = 0; i < pws.size(); i ++) {
          std::cout << pws[i].serialize() << std::endl;
        }
      }catch(const utils::InvalidCipherTypeException& e) {
        std::cout << e.what() << std::endl;
      }
    };

  private:
    core::PasswordFile f = core::PasswordFile(clib::String("./test"));
  };
}
