#include "VignereCipher.h"
#include "../CipherTypeUtils.h"
#include "../../gui/Shell.h"
#include<iostream>

namespace encrypt {
  VignereCipher::VignereCipher(const clib::String& key)
   : key(toZ96(key)) { }

  VignereCipher::VignereCipher(const clib::List<math::Z96>& key)
   : key(key) { };

  clib::String VignereCipher::encrypt(const clib::String& text) const {
    clib::List<math::Z96> result;
    clib::List<math::Z96> tokens = toZ96(text);
    for(std::size_t i = 0; i < text.size(); i ++) {
      result.add(tokens[i] + key[i % key.size()]);
    }
    return fromZ96(result);
  }
  clib::String VignereCipher::decrypt(const clib::String& text) const {
    clib::List<math::Z96> result;
    clib::List<math::Z96> tokens = toZ96(text);
    for(std::size_t i = 0; i < text.size(); i ++) {
      result.add(tokens[i] - key[i % key.size()]);
    }
    return fromZ96(result);
  }

  clib::String VignereCipher::serialize() const {
    return fromZ96(key);
  }
  clib::String VignereCipher::type() const { 
    return "vignere_cipher";
  }

  VignereCipher* VignereCipher::clone() const {
    return new VignereCipher(key);
  }

  clib::List<math::Z96> VignereCipher::toZ96(const clib::String& text) {
    clib::List<math::Z96> res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      char c = text[i];
      if(c < ' ' || c > '~') {
        throw std::runtime_error("VignereCipher alphabet consists of only printable characters!");
      }
      res.add(math::Z96(signed(c - ' ')));
    }
    return res;
  }

  clib::String VignereCipher::fromZ96(const clib::List<math::Z96> tokens) {
    clib::String res;
    for(auto& code: tokens) {
      res += char(code.raw() + ' ');
    }
    return res;
  }

  VignereCipher* VignereCipherFactory::fromArgs(const clib::List<clib::String>& args) const {
    return new VignereCipher(args[0]);
  }
  VignereCipher* VignereCipherFactory::fromShell(gui::Shell& shell) const {
    clib::String key;
    shell.print_line("Enter vignere cipher keyword: ");
    getLine(shell.in(), key);
    return new VignereCipher(key);
  }
  clib::String VignereCipherFactory::type() const {
    return "vignere_cipher";
  }
}
