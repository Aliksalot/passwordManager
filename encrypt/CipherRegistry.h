#pragma once

#include"../utils/darray.h"
#include"../utils/mystring.h"
#include"Cipher.h"
#include"CipherFactory.h"

namespace encrypt {

  class CipherRegistry {
  private:
    clib::darray<CipherFactory*> list;

    void add(CipherFactory* cf);
  public:
    CipherRegistry();
    ~CipherRegistry();

    const CipherFactory* byType(clib::Text type) const;

    CipherRegistry(const CipherRegistry&) = delete;
    CipherRegistry& operator=(const CipherRegistry&) = delete;
    CipherRegistry(CipherRegistry&&) = delete;
    CipherRegistry& operator=(CipherRegistry&&) = delete;

    static const CipherRegistry& get();

    clib::darray<clib::Text> cipherTypes() const;

  };
}
