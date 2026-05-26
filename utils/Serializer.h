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
    static clib::List<core::PasswordEntry> deserializePasswords (const clib::String& raw) {
      clib::List<core::PasswordEntry> l;
      auto lines = raw.split('\n');
      for(std::size_t i = 0; i < lines.size(); i ++) {
        auto line = lines[i].split(' ');
        if(line.size() < 4) 
          throw std::invalid_argument("File possibly corrupted, line format is invalid!");

        clib::List<clib::String> cipherArguments;
        for(std::size_t j = 4; j < line.size(); j ++) {
          cipherArguments.add(line[j]);
        }

        auto cipher = encrypt::CipherFactory::create(
          encrypt::CipherConfig(stringToCipherType(line[3]), cipherArguments)
        );

        l.add(core::PasswordEntry(
          line[0], line[1], line[2],
          cipher
        ));
      }

      return l;
    }
    static clib::String serializePasswords (clib::List<core::PasswordEntry> data) {
      clib::String result;
      auto passCount = data.size();
      for(std::size_t i = 0; i < passCount; i ++) {
        const encrypt::Cypher* c = data[i].getCipher();
        result +=
          data[i].getWebsite() + " " + data[i].getUsername() + " " + data[i].getPasswordEncrypted()
            + " " + cipherTypeToString(c->type()) + " " + c->serialize() + "\n";
      }
      return result;
    }
    static clib::String serializeHeader (
      const encrypt::Cypher* c
    ) {
      clib::String header = cipherTypeToString(c->type()) + " " + c->serialize();
      return header;
    }

    static encrypt::CipherConfig deserializeHeader (clib::String raw) {
      clib::List<clib::String> args = raw.split(' ');
      if(args.size() == 0)
        throw utils::InvalidCipherTypeException("File header possibly corrupted");
      encrypt::CipherType type = stringToCipherType(args[0]);
      args.remove(0);
      return encrypt::CipherConfig(type, args);
    }

    static clib::String cipherTypeToString (encrypt::CipherType t) {
      switch(t) {
        case encrypt::CipherType::TestCipher: return clib::String("test_cypher");
        case encrypt::CipherType::CaesarCipher: return clib::String("caesar_cypher");
        default: 
          throw utils::InvalidCipherTypeException();
      }
    }
    static encrypt::CipherType stringToCipherType (const clib::String& s) {
      if(s == cipherTypeToString(encrypt::CipherType::TestCipher))
        return encrypt::CipherType::TestCipher;
      if(s == cipherTypeToString(encrypt::CipherType::CaesarCipher))
        return encrypt::CipherType::CaesarCipher;

      throw utils::InvalidCipherTypeException();
    }
  };
}
