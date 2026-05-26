#include<iostream>
#include"./encrypt/TestCypher.h"
#include"./core/PasswordFile.h"
#include"./core/PasswordManager.h"
#include"./utils/string.h"
#include"./utils/Serializer.h"

int main(void) {
  
  encrypt::Cypher* c = new encrypt::TestCypher(clib::String("Hello"));

  clib::String s = utils::Serializer::serializeHeader(c);
  std::cout << s << std::endl;

  encrypt::Cypher* c1 = utils::Serializer::deserializeHeader(s);

  std::cout << utils::Serializer::cipherTypeToString(c1->type()) << std::endl;
  std::cout << c1->serialize() << std::endl;

  delete c;
  delete c1;

  //std::cout << tc.encrypt("Mainatati debelako") << std::endl;
  //core::PasswordManager pm;


  return 0;
}
