#include"HillCipher.h"

#include"../Cipher.h"
#include"../../math/SqMatrix.h"
#include"../../math/RemRing.h"
#include"../../gui/Shell.h"
#include"../../utils/exceptions.h"
#include"../../utils/mystring.h"

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

  /* Since proccess of initializing a HillCipher is quite annoying, it can be aborted and
   * a nullptr can be returned. Must be handled */
  HillCipher* HillCipherFactory::fromShell(gui::Shell& shell) const {
    //TODO maybe its better someone else to be responsible for building a matrix from Shell.
    //Perhaps matrix factory of sorts
    while(1) {
      shell.print_line("Enter matrix dimension (Or type \"abort\" to abort): ");
      clib::Text dimStr;
      clib::getLine(shell.in(), dimStr);
      dimStr.trimInPlace();
      if(dimStr == "abort") {
        return nullptr;
      }
      if(!dimStr.isValidInt()) {
        shell.print_line("Should be valid integer!");
        continue;
      }
      std::size_t dim = dimStr.toInt();

      math::DataList dl;
      for(std::size_t r = 0; r < dim; r++) {
        dl.add(clib::darray<math::Z26>());
        shell.print_line("Enter row " + clib::Text::fromInt(r) + " (Separate with spaces): ");
        clib::Text line;
        clib::getLine(shell.in(), line);
        auto tokens = line.trimInPlace().split(' ');
        if(tokens.size() != dim) {
          shell.print_line("Row must have exactly dim elements");
          r--;
          dl.pop();
          continue;
        }
        
        bool result = true;
        for(std::size_t c = 0; c < dim; c++) {
          if(!tokens[c].isValidInt()) {
            shell.print_line("All elements should be valid integers!");
            result = false;
            break;
          }
          dl[r].add((int)tokens[c].toInt());
        }
        if(!result) { 
          r --;
          dl.pop();
          continue;
        }
      }
      try{
        math::SqMatrix m(dl); 
        return new HillCipher(m);
      }catch(const utils::MathError& e){
        shell.print_line("Matrix is probably not invertable. Try again.");
      }catch(const std::runtime_error& e) {
        shell.print_line(e.what());
      }
    }
  }

  clib::Text HillCipherFactory::type() const {
    return "hill_cipher";
  }
}
