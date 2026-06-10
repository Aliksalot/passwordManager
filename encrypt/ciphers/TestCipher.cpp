#include"TestCipher.h"
#include"../../gui/Shell.h"

#include<iostream>

namespace encrypt {
  TestCipher::TestCipher(const clib::Text& secretword): secretword(secretword) { }

  clib::Text TestCipher::type() const { 
    return "test_cipher";
  }

  clib::Text TestCipher::serialize() const {
    return secretword;
  }

  clib::Text TestCipher::encrypt(const clib::Text& text) const{
    return text + secretword;
  };
  clib::Text TestCipher::decrypt(const clib::Text& text) const{
    return text;
  };
  TestCipher* TestCipher::clone() const {
    return new TestCipher(secretword);
  }

  clib::Text TestCipher::fromPassword(const clib::Text& password) {
    return password;
  }

  TestCipher* TestCipherFactory::fromArgs(const clib::darray<clib::Text>& args) const {
    return new TestCipher(args[0]);
  }
  TestCipher* TestCipherFactory::fromShell(gui::Shell& shell) const {
    clib::Text key;
    shell.in() >> key;
    return new TestCipher(key);
  }
  clib::Text TestCipherFactory::type() const {
    return "test_cipher";
  }
}
