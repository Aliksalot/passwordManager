#include"CipherRegistry.h"
#include"../utils/darray.h"
#include"../utils/mystring.h"
#include"Cipher.h"
#include"CipherFactory.h"
#include"../utils/exceptions.h"

#include"./ciphers/TestCipher.h"
#include"./ciphers/HillCipher.h"
#include"./ciphers/CaesarCipher.h"
#include"./ciphers/VignereCipher.h"
#include"./ciphers/TextCode.h"

namespace encrypt {

  CipherRegistry::CipherRegistry() {
    add(new TestCipherFactory());
    add(new HillCipherFactory());
    add(new CaesarCipherFactory());
    add(new VignereCipherFactory());
    add(new TextCodeFactory());
  }

  void CipherRegistry::add(CipherFactory* cf) {
    if(cf != nullptr){
      list.add(cf);
    }
  }

  CipherRegistry::~CipherRegistry() {
    for(auto& cf: list) {
      delete cf;
    }
  }

  const CipherFactory* CipherRegistry::byType(clib::Text type) const {
    for(auto& cf: list) {
      if(cf->type() == type) {
        return cf;
      }
    }

    throw utils::InvalidCipherTypeError(clib::Text("Invalid cipher: ") + type + "\nNote: If you \
are a developer and think this is a mistake, check if you have forgotten to register a new \
CipherType");
  }

  const CipherRegistry& CipherRegistry::get() {
    static CipherRegistry cf;
    return cf;
  }

  clib::darray<clib::Text> CipherRegistry::cipherTypes() const {
    clib::darray<clib::Text> types;
    for(auto& factory: list) {
      types.add(factory->type());
    }
    return types;
  }
}

