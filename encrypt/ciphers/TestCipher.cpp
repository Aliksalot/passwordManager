#include"TestCipher.h"
#include"../../gui/Shell.h"

#include<iostream>

namespace encrypt {
  TestCipher::TestCipher(const clib::String& secretword): secretword(secretword) { }

  clib::String TestCipher::type() const { 
    return "test_cipher";
  }

  clib::String TestCipher::serialize() const {
    return secretword;
  }

  clib::String TestCipher::encrypt(const clib::String& text) const{
    return text + secretword;
  };
  clib::String TestCipher::decrypt(const clib::String& text) const{
    return text;
  };
  TestCipher* TestCipher::clone() const {
    return new TestCipher(secretword);
  }

  clib::String TestCipher::fromPassword(const clib::String& password) {
    return password;
  }

  TestCipher* TestCipherFactory::fromArgs(const clib::List<clib::String>& args) const {
    return new TestCipher(args[0]);
  }
  TestCipher* TestCipherFactory::fromShell(gui::Shell& shell) const {
    clib::String key;
    shell.in() >> key;
    return new TestCipher(key);
  }
  clib::String TestCipherFactory::type() const {
    return "test_cipher";
  }
}
