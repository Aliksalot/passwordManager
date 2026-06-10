#pragma once

#include"../../utils/array.h"
#include"../../utils/string.h"
#include"../../math/RemRing.h"
#include"../Cipher.h"
#include"../CipherFactory.h"

namespace encrypt {
  class VignereCipher : public Cipher {
  private:
    clib::List<math::Z96> key;
  public:
    VignereCipher(const clib::String& key);
    VignereCipher(const clib::List<math::Z96>& key);

    clib::String encrypt(const clib::String& text) const override;
    clib::String decrypt(const clib::String& text) const override;

    clib::String serialize() const override;
    clib::String type() const override;

    VignereCipher* clone() const override;

    static clib::List<math::Z96> toZ96(const clib::String& text);
    static clib::String fromZ96(const clib::List<math::Z96> tokens);
  };

  class VignereCipherFactory: public CipherFactory {
  public:
    VignereCipher* fromArgs(const clib::List<clib::String>& args) const override;
    VignereCipher* fromShell(gui::Shell& shell) const override;
    clib::String type() const override;
  };
}
