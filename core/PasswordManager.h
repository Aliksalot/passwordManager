#pragma once

#include<iostream>

#include"./PasswordFile.h"
#include"./Password.h"
#include"../encrypt/TestCypher.h"
#include<iostream>

namespace core{
  
  class PasswordManager{

  public:
    PasswordManager() {
      /*
      f.createFile(
        new encrypt::TestCypher("Helloa"),
        "password"
      );

      f.add("google.com", "u1", "password1");
      f.add("google.com", "u2", "password2");

      f.save("password");
      */

      try{
        f.load("password");
        auto pws = f.find();
      }catch(const utils::InvalidCipherTypeException& e) {
        std::cout << e.what() << std::endl;
      }
    };

  private:
    core::PasswordFile f = core::PasswordFile(clib::String("./test"));
  };
}
