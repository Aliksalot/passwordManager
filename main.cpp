#include<iostream>
#include"./encrypt/TestCypher.h"
#include"./core/PasswordFile.h"
#include"./core/PasswordManager.h"
#include"./utils/string.h"

int main(void) {
  
  //encrypt::TestCypher tc = encrypt::TestCypher(clib::String("Hello"));
  //encrypt::Cypher& c = tc;
  //std::cout << tc.encrypt("Mainatati debelako") << std::endl;
  core::PasswordManager pm;

  clib::String a = "asd";
  clib::String b = a;
  std::cout << a << " " << b << std::endl;

  return 0;
}
