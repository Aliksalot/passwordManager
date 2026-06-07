#include"Shell.h"

#include"../utils/string.h"
#include"../utils/array.h"

#include<iostream>

namespace gui {
  
  void Shell::open(const TokenList& t) {
    if(t.size() < 2){
      std::cout << "Invalid usage -> open <filename> <password>" << std::endl;
      return;
    }
    
    pm.openFile(t[0], t[1]);
  }

  void Shell::create(const TokenList& t) {
    if(t.size() < 3) {
      std::cout << "Invalid usage -> create <filename> <cipher> <password>" << std::endl;
      return;
    }
    
    try{
      encrypt::CipherType ct = encrypt::stringToCipherType(t[1]);
    }catch(const utils::InvalidCipherTypeError& e) {
      std::cout << e.what() << std::endl;
      std::cout << "You must chose from one of the predefined cipher types: " << std::endl;
      encrypt::listCipherTypes();
    };

  }
}
