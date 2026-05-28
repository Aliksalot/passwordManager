#pragma once

#include"../core/PasswordFile.h"
#include"../core/Password.h"
#include"../encrypt/CipherTypeUtils.h"
#include"../encrypt/Cypher.h"
#include"../encrypt/CipherFactory.h"
#include"../utils/exceptions.h"
#include<stdexcept>

namespace utils {
  class Serializer {
  public:
    static clib::List<core::PasswordEntry> deserializePasswords (const clib::String& raw) {
      return deserializePasswords(raw.split('\n'));
    }

    static clib::List<core::PasswordEntry> deserializePasswords (
        const clib::List<clib::String>& lines
    ) {

      clib::List<core::PasswordEntry> l;
      for(std::size_t i = 0; i < lines.size(); i ++) {

        if(lines[i].empty()) continue;

        auto line = lines[i].split(' ');

        if(line.empty()) continue;

        if(line.size() < 4) 
          throw std::invalid_argument("File possibly corrupted, line format is invalid!");

        clib::List<clib::String> cipherArguments;
        for(std::size_t j = 4; j < line.size(); j ++) {
          cipherArguments.add(line[j]);
        }

        auto cipher = encrypt::CipherFactory::create(
          encrypt::stringToCipherType(line[3]), cipherArguments
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
      std::size_t passCount = data.size();
      for(std::size_t i = 0; i < passCount; i ++) {
        result += data[i].serialize();
        result += "\n";
      }
      return result;
    }
    static clib::String serializeCipher (const encrypt::Cypher* c) {
      return encrypt::cipherTypeToString(c->type()) + " " + c->serialize();
    }

    static encrypt::Cypher* deserializeCipher (const clib::String& raw) {
      clib::List<clib::String> words = raw.split(' ');
      if(words.size() == 0)
        throw utils::InvalidCipherTypeException("File possibly corrupted");
      encrypt::CipherType type = encrypt::stringToCipherType(words[0]);
      clib::List<clib::String> args;
      for(std::size_t i = 1; i < words.size(); i ++) {
        args.add(words[i]);
      }
      return encrypt::CipherFactory::create(type, args);
    }

  };
}
