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
     clib::String encrypt(const clib::String& text) const override;
     clib::String decrypt(const clib::String& text) const override;

     clib::String serialize() const override;
     clib::String type() const override;

     HillCipher* clone() const override;

     HillCipher() = delete;
     HillCipher(math::SqMatrix m);

     static clib::List<math::Z26> toZ26(const clib::String& text);
     static clib::String fromZ26(const clib::List<math::Z26> tokens);
  };

  class HillCipherFactory: public CipherFactory {
  public:
    HillCipher* fromArgs(const clib::List<clib::String>& args) const override;
    HillCipher* fromCin() const override;
    clib::String type() const override;
  };

}
