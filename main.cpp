#include<iostream>
#include"./core/PasswordFile.h"
#include"./encrypt/ciphers/CaesarCipher.h"
#include"./encrypt/ciphers/TestCipher.h"
#include"./encrypt/ciphers/TextCode.h"
#include"./encrypt/ciphers/HillCipher.h"
#include"./encrypt/ciphers/VignereCipher.h"
#include"./utils/string.h"
#include"./math/RemRing.h"
#include"./math/SqMatrix.h"

#include"./gui/Shell.h"

void run() {
  char lineRaw[1024];
  gui::Shell shell;

  shell.print_line("Welcome to password manager. If you are new type \"help\" for  a list of commands.");
  while(1) {
    shell.in().getline(lineRaw, 1024);

    clib::String line = lineRaw;
    
    clib::List<clib::String> tokens = line.split(' ');
    clib::String cmd = tokens[0];
    tokens.remove(0);

    if(cmd == "open") shell.open(tokens);
    else if(cmd == "create") shell.create(tokens);
    else if(cmd == "close") shell.close();
    else if(cmd == "save_file") shell.save_file();
    else if(cmd == "save") shell.save(tokens);
    else if(cmd == "load") shell.load(tokens);
    else if(cmd == "update") shell.update(tokens);
    else if(cmd == "delete") shell.entryDelete(tokens);
    else if(cmd == "list") shell.list();
    else if(cmd == "ciphers") shell.listCiphers();
    else if(cmd == "exit") { 
      shell.close();
      shell.print_line("Goodbye!");
      break;
    }
    else if(cmd == "help") shell.help();
    else {
      shell.print_line("Invalid command. Check \"help\" for a list of commands");
    }
  }
}

void test() {
  core::PasswordFile f("myfilexd");
  
  f.createFile(
    new encrypt::CaesarCipher(22),
    "password"
  );

  math::DataList dl;
  dl.add(clib::List<math::Z26>());
  dl.add(clib::List<math::Z26>());
  dl[0].add(3);
  dl[0].add(3);
  dl[1].add(2);
  dl[1].add(7);

  math::SqMatrix m = math::SqMatrix(dl);

  f.add("google.com", "u1", "password1", new encrypt::TestCipher("Hello"));
  f.add("google.com", "u2", "password2", new encrypt::TextCode("./data/encryption"));
  f.add("google.com", "u3", "password3", new encrypt::CaesarCipher(5));
  f.add("google.com", "u4", "password", new encrypt::HillCipher(m));
  f.add("google.com", "u5", "ilovecoffee", new encrypt::VignereCipher("iwanttokissyourfeet"));

  f.save("password");
  try{
    f.load("password");
    std::cout << f.getDefaultCipher()->type() << " " << f.getDefaultCipher()->serialize() << std::endl;
    auto pws = f.find();
    for(auto& p: pws) {
      std::cout << p->serialize() << std::endl;
      std::cout << "Decrypted: " << p->getCipher()->decrypt(p->getPasswordEncrypted()) << std::endl;
    }
  }catch(const utils::EncryptionError& e) {
    std::cout << e.what() << std::endl;
  }catch(const utils::FileError& e) {
    std::cout << e.what() << std::endl;
  }
}

int main(void) {
  run();

  return 0;
}
