#include"HillCipher.h"

#include"../Cipher.h"
#include"../../math/SqMatrix.h"
#include"../../math/RemRing.h"
#include"../../gui/Shell.h"
#include"../../utils/exceptions.h"

namespace encrypt {
  HillCipher::HillCipher(math::SqMatrix m): m(m) { };

  clib::Text HillCipher::serialize() const {
    return m.serialize();
  }

  clib::Text HillCipher::type() const {
    return "hill_cipher";
  }

  HillCipher* HillCipher::clone() const {
    return new HillCipher(m);
  }

  clib::Text HillCipher::encrypt(const clib::Text& text) const {
    return fromZ26(m.apply(toZ26(text)));
  }

  clib::Text HillCipher::decrypt(const clib::Text& text) const {
    return fromZ26(m.applyInverse(toZ26(text)));
  }

  clib::darray<math::Z26> HillCipher::toZ26(const clib::Text& text) {
    clib::darray<math::Z26> res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      char c = text[i];
      if(c < 'a' || c > 'z')
        throw std::runtime_error("HillCipher alphabet consists of only a-z lowercase!");
      res.add(math::Z26(signed(c - 'a')));
    }
    return res;
  }

  clib::Text HillCipher::fromZ26(const clib::darray<math::Z26> tokens) {
    clib::Text res;
    for(auto& code: tokens) {
      res += char(code.raw() + 'a');
    }
    return res;
  }


  HillCipher* HillCipherFactory::fromArgs(const clib::darray<clib::Text>& args) const {
    return new HillCipher(math::SqMatrix::deserialize(args[0]));
  }

  HillCipher* HillCipherFactory::fromShell(gui::Shell& shell) const {
    while(1) {
      shell.print_line("Enter matrix dimension: ");
      clib::Text dimStr;
      clib::getLine(shell.in(), dimStr);
      std::size_t dim = dimStr.toInt();

      math::Datadarray dl;
      for(std::size_t r = 0; r < dim; r++) {
        dl.add(clib::darray<math::Z26>());
        shell.print_line("Enter row " + clib::Text::fromInt(r) + " (Separate with spaces): ");
        clib::Text line;
        clib::getLine(shell.in(), line);
        auto tokens = line.split(' ');
        if(tokens.size() != dim) {
          shell.print_line("Row must have exactly dim elements");
          r--;
          continue;
        }
        for(std::size_t c = 0; c < dim; c++) {
          dl[r].add((int)tokens[c].toInt());
        }
      }
      try{
        math::SqMatrix m(dl); 
        return new HillCipher(m);
      }catch(const utils::MathError& e){
        shell.print_line("Matrix is probably not invertable. Try again.");
      }
    }
  }

  clib::Text HillCipherFactory::type() const {
    return "hill_cipher";
  }
}
