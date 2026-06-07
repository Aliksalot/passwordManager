#pragma once

#include"../utils/array.h"
#include"../utils/string.h"
#include"Cipher.h"
#include"CipherFactory.h"

namespace encrypt {

  class CipherRegistry {
  private:
    clib::List<CipherFactory*> list;

    void add(CipherFactory* cf);
  public:
    CipherRegistry();
    ~CipherRegistry();

    const CipherFactory* byType(clib::String type) const;

    CipherRegistry(const CipherRegistry&) = delete;
    CipherRegistry& operator=(const CipherRegistry&) = delete;
    CipherRegistry(CipherRegistry&&) = delete;
    CipherRegistry& operator=(CipherRegistry&&) = delete;

    static const CipherRegistry& get();

    clib::List<clib::String> cipherTypes() const;

  };
}
