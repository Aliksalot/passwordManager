#include "VignereCipher.h"
#include "../CipherTypeUtils.h"
#include "../../gui/Shell.h"
#include<iostream>

namespace encrypt {
  VignereCipher::VignereCipher(const clib::Text& key)
   : key(toZ96(key)) { }

  VignereCipher::VignereCipher(const clib::darray<math::Z96>& key)
   : key(key) { };

  clib::Text VignereCipher::encrypt(const clib::Text& text) const {
    clib::darray<math::Z96> result;
    clib::darray<math::Z96> tokens = toZ96(text);
    for(std::size_t i = 0; i < text.size(); i ++) {
      result.add(tokens[i] + key[i % key.size()]);
    }
    return fromZ96(result);
  }
  clib::Text VignereCipher::decrypt(const clib::Text& text) const {
    clib::darray<math::Z96> result;
    clib::darray<math::Z96> tokens = toZ96(text);
    for(std::size_t i = 0; i < text.size(); i ++) {
      result.add(tokens[i] - key[i % key.size()]);
    }
    return fromZ96(result);
  }

  clib::Text VignereCipher::serialize() const {
    return fromZ96(key);
  }
  clib::Text VignereCipher::type() const { 
    return "vignere_cipher";
  }

  VignereCipher* VignereCipher::clone() const {
    return new VignereCipher(key);
  }

  clib::darray<math::Z96> VignereCipher::toZ96(const clib::Text& text) {
    clib::darray<math::Z96> res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      char c = text[i];
      if(c < ' ' || c > '~') {
        throw std::runtime_error("VignereCipher alphabet consists of only printable characters!");
      }
      res.add(math::Z96(signed(c - ' ')));
    }
    return res;
  }

  clib::Text VignereCipher::fromZ96(const clib::darray<math::Z96> tokens) {
    clib::Text res;
    for(auto& code: tokens) {
      res += char(code.raw() + ' ');
    }
    return res;
  }

  VignereCipher* VignereCipherFactory::fromArgs(const clib::darray<clib::Text>& args) const {
    return new VignereCipher(args[0]);
  }
  VignereCipher* VignereCipherFactory::fromShell(gui::Shell& shell) const {
    clib::Text key;
    shell.print_line("Enter vignere cipher keyword: ");
    getLine(shell.in(), key);
    return new VignereCipher(key);
  }
  clib::Text VignereCipherFactory::type() const {
    return "vignere_cipher";
  }
}
