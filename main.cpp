#include<iostream>
#include<stdexcept>
#include"./core/PasswordFile.h"
#include"./encrypt/ciphers/CaesarCipher.h"
#include"./encrypt/ciphers/TestCipher.h"
#include"./encrypt/ciphers/TextCode.h"
#include"./encrypt/ciphers/HillCipher.h"
#include"./encrypt/ciphers/VignereCipher.h"
#include"./utils/mystring.h"
#include"./math/RemRing.h"
#include"./math/SqMatrix.h"

#include"./gui/Shell.h"

int run() {
  char lineRaw[1024];
  gui::Shell shell;

  shell.print_line("Welcome to password manager. If you are new type \"help\" for  a list of commands.");
  while(1) {

    if(!shell.isRunning()) {
      shell.print_line("Goodbye!");
      return 0;
    }
    shell.in().getline(lineRaw, 1024);

    clib::Text line = lineRaw;

    clib::darray<clib::Text> tokens = line.trim().split(' ');

    if(tokens.empty()) continue;

    clib::Text cmd = tokens[0];
    tokens.remove(0);

    shell.execute(cmd, tokens);
  }

  throw std::logic_error("Unreachable");
}

void test() {
  core::PasswordFile f("myfilexd");
  
  f.createFile(
    new encrypt::CaesarCipher(22),
    "password"
  );

  math::DataList dl;
  dl.add(clib::darray<math::Z26>());
  dl.add(clib::darray<math::Z26>());
  dl[0].add(3);
  dl[0].add(3);
  dl[1].add(2);
  dl[1].add(7);

  math::SqMatrix m = math::SqMatrix(dl);

  f.add("google.com", "u1", "password1", new encrypt::TestCipher("Hello"));
  f.add("google.com", "u2", "password2", new encrypt::TextCode("./data/encryption"));
  f.add("google.com", "u3", "password3", new encrypt::CaesarCipher(5));
  f.add("google.com", "u4", "password", new encrypt::HillCipher(m));
  f.add("google.com", "u5", "ilovecoffee", new encrypt::VignereCipher("vingeretestkey"));

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
  return run();
}
