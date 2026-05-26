#pragma once

#include"./PasswordFile.h"
#include"./Password.h"
#include"../encrypt/TestCypher.h"
#include<iostream>

namespace core{
  
  class PasswordManager{

  public:
    PasswordManager() {
      f.update(PasswordEntry("google.com", "alex", "vabdsfudshfga9uhdsjjfijqifjfjwqiisajdjsalkjdklzx", new encrypt::TestCypher("penis")));
      f.update(PasswordEntry("google.com", "totev", "chepping", new encrypt::TestCypher("chep")));
      f.update(PasswordEntry("google.com", "totev", "Kur", new encrypt::TestCypher("chep")));
      clib::List<PasswordEntry> l = f.find();
      for(std::size_t i = 0; i < l.size(); i ++) {
        std::cout << l[i].getWebsite() << " " << l[i].getUsername() << std::endl;
      }
    };

  private:
    core::PasswordFile f;

  };
}
