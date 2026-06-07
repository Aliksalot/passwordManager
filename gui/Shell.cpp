#include"Shell.h"

#include"../utils/string.h"
#include"../utils/array.h"
#include"../encrypt/CipherRegistry.h"

#include<iostream>

namespace gui {
  
  void Shell::open(const TokenList& t) {
    if(t.size() < 2){
      print_line("Invalid usage -> open <filename> <password>");
      return;
    }
    
    pm.openFile(t[0], t[1]);
  }

  void Shell::create(const TokenList& t) {
    if(t.size() < 3) {
      print_line("Invalid usage -> create <filename> <cipher_type> <password>");
      return;
    }
    
    try{
      
      encrypt::Cipher* defaultCipher = encrypt::CipherRegistry::get()
        .byType(t[1])->fromCin();

      pm.createFile(t[0], defaultCipher, t[2]);

    }catch(const utils::InvalidCipherTypeError& e) {
      print_line(e.what());
      print_line("You must chose from one of the predefined cipher types: ");
      for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
        print_line(ct);
      }
    };
  }

  void Shell::close() {
    if(pm.isFileDirty()) {
      if(promptSave()) {
        pm.saveFile();
      }
    }
    pm.closeFile();
  }

  void Shell::save(const TokenList& t) {
    if(t < 2) {
      print_line("Invalid usage -> save <website> <user> [<cipher> <password>]");
    }
    pm
  }

  bool Shell::promptSave() {
    print_line("File has unsaved changes. Do you wish to save before leaving?");
    clib::String w;
    do{
      print_line("(y)es or (n)o");
      in() >> w;
      if(w[0] == 'y')
        return true;
      if(w[0] == 'n')
        return false;
    }while(true);
  }

  std::istream& Shell::in() {
    std::cout << "pm < ";
    return std::cin;
  }

  void Shell::print(const clib::String& s) {
    std::cout << "pm > " << s;
  }
  void Shell::print_line(const clib::String& s, char eol) {
    std::cout << "pm > " << s << eol;
  }

}
