#pragma once

#include"../../utils/darray.h"
#include"../../utils/mystring.h"
#include"../../math/RemRing.h"
#include"../Cipher.h"
#include"../CipherFactory.h"

namespace encrypt {
  class VignereCipher : public Cipher {
  private:
    clib::darray<math::Z96> key;
  public:
    VignereCipher(const clib::Text& key);
    VignereCipher(const clib::darray<math::Z96>& key);

    clib::Text encrypt(const clib::Text& text) const override;
    clib::Text decrypt(const clib::Text& text) const override;

    clib::Text serialize() const override;
    clib::Text type() const override;

    VignereCipher* clone() const override;

    static clib::darray<math::Z96> toZ96(const clib::Text& text);
    static clib::Text fromZ96(const clib::darray<math::Z96> tokens);
  };

  class VignereCipherFactory: public CipherFactory {
  public:
    VignereCipher* fromArgs(const clib::darray<clib::Text>& args) const override;
    VignereCipher* fromShell(gui::Shell& shell) const override;
    clib::Text type() const override;
  };
}
