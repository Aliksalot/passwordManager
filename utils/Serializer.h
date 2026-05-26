#pragma once

#include"../core/PasswordFile.h"
#include"../core/Password.h"
#include"../encrypt/Cypher.h"
#include"../encrypt/CipherTypes.h"
#include<stdexcept>

namespace utils {
  class Serializer {
    static clib::List<core::PasswordEntry> deserializePasswords (clib::String raw) {
      return clib::List<core::PasswordEntry>();
    }
    static clib::String serializePasswords (clib::List<core::PasswordEntry> data) {
      return clib::String("");
    }
    static clib::String serializeHeader (const encrypt::Cypher* c) {
      String header = cipherTypeToString(c->type()) + " " + c->serialize();
      return header
    }
    static encrypt::Cypher* deserializeHeader (clib::String raw) {
      //TODO
      //needs to determine what type of cypher it is and construct the object accordingly
      return nullptr;
    }
    static clib::String cipherTypeToString (encrypt::CipherType t) {
      switch(t) {
        case encrypt::CipherType::TestCipher: return clib::String("test_cypher");
        default: 
          throw std::invalid_argument("Invalid CipherType");
      }
    }
    static encrypt::CipherType stringToCipherType (const clib::String& s) {
      if(s == cipherTypeToString(encrypt::CipherType::TestCipher))
        return encrypt::CipherType::TestCipher;

      throw std::invalid_argument("Invalid CipherType");
    }
  };
}
