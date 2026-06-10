#include"CaesarCipher.h"
#include"../../utils/darray.h"
#include"../../utils/mystring.h"
#include"../../gui/Shell.h"

namespace encrypt{

  CaesarCipher::CaesarCipher(unsigned step): step(step) { }

  clib::Text CaesarCipher::type() const { 
    return "caesar_cipher";
  }

  clib::Text CaesarCipher::serialize() const {
    return clib::Text::fromInt(step);
  }

  clib::Text CaesarCipher::encrypt(const clib::Text& text) const{
    clib::Text res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      signed c = static_cast<unsigned char>(text[i]);

      if(c < CAESAR_RANGE_ST || c > CAESAR_RANGE_END) 
        throw std::invalid_argument("Character out of Caesar's range");

      c -= CAESAR_RANGE_ST;
      c = (c + step) % CAESAR_ALPHABET_SIZE;
      res += (char(c + CAESAR_RANGE_ST));
    }
    
    return res;
  };
  clib::Text CaesarCipher::decrypt(const clib::Text& text) const{
    clib::Text res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      signed c = static_cast<unsigned char>(text[i]);

      if(c < CAESAR_RANGE_ST || c > CAESAR_RANGE_END) 
        throw std::invalid_argument("Character out of Caesar's range");

      c = (c - CAESAR_RANGE_ST - step);
      if(c < 0) c += CAESAR_ALPHABET_SIZE;
      c %= CAESAR_ALPHABET_SIZE;
      res += (char(c + CAESAR_RANGE_ST));
    }
    
    return res;
  };
  CaesarCipher* CaesarCipher::clone() const {
    return new CaesarCipher(step);
  }

  CaesarCipher* CaesarCipherFactory::fromArgs(const clib::darray<clib::Text>& args) const {
    return new CaesarCipher(atoi(args[0].raw()));
  }
  CaesarCipher* CaesarCipherFactory::fromShell(gui::Shell& shell) const {
    clib::Text input;
    shell.print_line("Ender caesar cipher shift (whole number): ");
    std::cin >> input;
    return new CaesarCipher(atoi(input.raw()));
  }
  clib::Text CaesarCipherFactory::type() const {
    return "caesar_cipher";
  }
}
