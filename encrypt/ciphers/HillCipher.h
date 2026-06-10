#pragma once

#include"../CipherFactory.h"
#include"../Cipher.h"
#include"../../math/SqMatrix.h"
#include"../../math/RemRing.h"

namespace encrypt {

  class HillCipher: public Cipher {
  private:
    math::SqMatrix m;
  public:
     clib::Text encrypt(const clib::Text& text) const override;
     clib::Text decrypt(const clib::Text& text) const override;

     clib::Text serialize() const override;
     clib::Text type() const override;

     HillCipher* clone() const override;

     HillCipher() = delete;
     HillCipher(math::SqMatrix m);

     static clib::darray<math::Z26> toZ26(const clib::Text& text);
     static clib::Text fromZ26(const clib::darray<math::Z26> tokens);
  };

  class HillCipherFactory: public CipherFactory {
  public:
    HillCipher* fromArgs(const clib::darray<clib::Text>& args) const override;
    HillCipher* fromShell(gui::Shell& shell) const override;
    clib::Text type() const override;
  };

}
