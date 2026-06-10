#include"Shell.h"

#include"../utils/string.h"
#include"../utils/array.h"
#include"../core/Password.h"
#include"../core/PasswordView.h"
#include"../encrypt/CipherRegistry.h"

#include<iostream>

namespace gui {

  Shell::Shell() { }

  const clib::String Shell::commandUsage(const clib::String& cmd) {
    if(cmd == "open")   return "open <filename> <password>";
    if(cmd == "create") return "create <filename> <cipher> <password>";
    if(cmd == "save")   return "save <website> <username> [<cipher>] <password>";
    if(cmd == "load")   return "load <website> [<username>]";
    if(cmd == "update") return "update <website> <username> <new-password>";
    if(cmd == "delete") return "delete <website> [<username>]";

    return "";
  }
  
  void Shell::open(const TokenList& t) {
    if(t.size() < 2){
      print_line("Invalid usage -> " + commandUsage("open"));
      return;
    }
    
    if(pm.hasOpenFile()) {
      print_line("You already have an open file!");
      return;
    }

    try{
      pm.openFile(t[0], t[1]);
      print_line("File opened successfully.");
    }catch(const std::exception& e) {
      print_line(e.what());
    }catch(...) {
      print_line("Unknown error occured, while trying to open file");
    }
  }

  void Shell::listCiphers() const{

    for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
      print_line(ct);
    }
  }

  void Shell::create(const TokenList& t) {
    if(t.size() != 3) {
      print_line("Needs exactly 3 arguments: Invalid usage -> " + commandUsage("create"));
      return;
    }
    
    try{
      
      encrypt::Cipher* defaultCipher = encrypt::CipherRegistry::get()
        .byType(t[1])->fromCin();

      pm.createFile(t[0], defaultCipher, t[2]);

      print_line("File created successfully!");

    }catch(const utils::InvalidCipherTypeError& e) {
      print_line(e.what());
      print_line("You must chose from one of the predefined cipher types: ");
      for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
        print_line(ct);
      }
    };
  }

  void Shell::close() {
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }
    try{
      if(pm.isFileDirty()) {
        print_line("File has unsaved changes. Do you wish to save before leaving?");
        if(promptConfirm()) {
          pm.saveFile();
        }
      }
      pm.closeFile();
    }catch(const utils::NoFileOpenError& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Close shouldn't be called when there is no file" << std::endl;
    }
  }

  void Shell::save(const TokenList& t) {
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }
    try{
      if(t.size() <= 2) {
        print_line("Too few arguments: Invalid usage -> " + commandUsage("save"));
      }else if(t.size() == 3) {
        pm.newPassword(t[0], t[1], t[2]);
      }else if(t.size() == 4) {
        encrypt::Cipher* cipher = encrypt::CipherRegistry::get()
          .byType(t[2])->fromCin();

        if(!pm.newPassword(t[0], t[1], t[3], cipher)) {
          print_line("Couldn't add new password entry. ");
        }
      }else {
        print_line("Too many arguments: Invalid usage -> " + commandUsage("save"));
      }
    }catch(const utils::InvalidCipherTypeError& e) {
      print_line(e.what());
      print_line("You must chose from one of the predefined cipher types: ");
      for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
        print_line(ct);
      }
    }
  }

  void Shell::save_file() {
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }
    pm.saveFile();
  }

  bool Shell::promptConfirm() {
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
    if(pm.hasOpenFile()) {
      std::cout << pm.getFilePath() << " < ";
    }
    return std::cin;
  }

  void Shell::print(const clib::String& s) const {
    std::cout << "pm > ";
    if(pm.hasOpenFile()) {
      std::cout << pm.getFilePath() << " > ";
    }
    std::cout << s;
  }
  void Shell::print_line(clib::String s, char eol) const {
    print(s += eol);
  }
  void Shell::load(const TokenList& t) {
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }

    clib::List<const core::PasswordEntry*> matches;
    if(t.size() < 1) {
      print_line("Too few arguments: Invalid usage -> " + commandUsage("load"));
    }else if(t.size() == 1) {
      matches = pm.loadPassword(t[0]);
    }else if(t.size() == 2) {
      matches = pm.loadPassword(t[0], t[1]);
    }else {
      print_line("Too many arguments: Invalid usage -> " + commandUsage("load"));
    }

    for(auto& entry: matches) {
      print_line(entry->getUsername() + "\t" + entry->getCipher()->decrypt(entry->getPasswordEncrypted()));
    }
  }
  void Shell::update(const TokenList& t) {
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }

    if(t.size() != 3) {
      print_line("Expected exactly 3 arguments: Invalid usage -> " + commandUsage("update"));
      return;
    }

    try{
      pm.updatePassword(t[0], t[1], t[2]);
      
      print_line("Password successfully updated!");
    }catch(const utils::PasswordModifyError& e) {
      print_line(e.what());
      return;
    }

  }

  void Shell::entryDelete(const TokenList& t) {
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }

    if(t.size() < 1) {
      print_line("Too few arguments: Invalid usage -> " + commandUsage("update"));
    }else if(t.size() == 1) {
      const clib::String& website = t[0];
      print_line("Warning - you are about to delete all passwords for website " + website + ". Are you sure?");
      if(promptConfirm()) {
        pm.deletePassword(website);
      }
    }else if(t.size() == 2) {
      pm.deletePassword(t[0], t[1]);
    }else{
      print_line("Too many arguments: Invalid usage -> " + commandUsage("update"));
    }

  }
  void Shell::list() const{
    if(!pm.hasOpenFile()) {
      print_line("No file open");
      return;
    }

    clib::List<const core::PasswordView*> passwords =
      pm.list();

    for(auto& pass: passwords) {
      print_line(pass->getWebsite() + "\t" + pass->getUsername());
    }
  }
  void Shell::help() const{
    print_line("-----------------------------------------------------------");
    print_line("open <filename> <password> - Decrypts the file at <filename> with <password> and loads it, allowing you to modify its content.");
    print_line("create <filename> <cipher> <password> - Creates a new file at <filename>. <cipher> will be used as a default cipher to encrypt the password entries. <password> is used to protect the file. ");
    print_line("save <website> <username> [<cipher>] <password> - Creates a new password entry. If you don't provide [<cipher>] the default one will be used. ");
    print_line("load <website> [<username>] - Prints on screen all password entries that match your query with passwords DECRYPTED. ");
    print_line("update <website> <username> <new-password> - Updates entry. ");
    print_line("delete <website> [<username>] - Deletes one or multiple entries. If you don't provide a [<username>], you will be prompted to delete all entries from <website>.");
    print_line("list - Prints on screen all entries WITHOUT sensitive data (only website and username). ");
    print_line("close - Closes current file. If it's dirty, you will be prompted. ");
    print_line("exit - Exits the program. If current file is dirty, you will be prompted to save it ");
    print_line("help - How did you get here?");
    print_line("save_file - Kind of a utility funciton. Saves the currently opened file, but you shall never need to call it manually.");
    print_line("ciphers - Provides a list of all implemented ciphers. ");
    print_line("-----------------------------------------------------------");
  }

}
