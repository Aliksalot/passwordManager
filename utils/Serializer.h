#pragma once

#include"../core/PasswordFile.h"
#include"../core/Password.h"
#include"../encrypt/Cypher.h"
#include"../encrypt/CipherTypes.h"
#include"../encrypt/CipherFactory.h"
#include"../utils/exceptions.h"
#include<stdexcept>

namespace utils {
  class Serializer {
  public:
    static clib::List<core::PasswordEntry> deserializePasswords (clib::String raw) {
      return clib::List<core::PasswordEntry>();
    }
    static clib::String serializePasswords (clib::List<core::PasswordEntry> data) {
      return clib::String("");
    }
    static clib::String serializeHeader (const encrypt::Cypher* c) {
      clib::String header = cipherTypeToString(c->type()) + " " + c->serialize();
      return header;
    }
    static encrypt::Cypher* deserializeHeader (clib::String raw) {
      clib::List<clib::String> args = raw.split(' ');

      if(args.size() == 0)
        throw utils::InvalidCipherTypeException("File header possibly corrupted");
      encrypt::CipherType type = stringToCipherType(args[0]);
      //TODO Little sloppy but ok, maybe fix if enough time
      args.remove(0);

      return encrypt::CipherFactory::create(type, args);
    }
    static clib::String cipherTypeToString (encrypt::CipherType t) {
      switch(t) {
        case encrypt::CipherType::TestCipher: return clib::String("test_cypher");
        default: 
          throw utils::InvalidCipherTypeException();
      }
    }
    static encrypt::CipherType stringToCipherType (const clib::String& s) {
      if(s == cipherTypeToString(encrypt::CipherType::TestCipher))
        return encrypt::CipherType::TestCipher;

      throw utils::InvalidCipherTypeException();
    }
  };
}
