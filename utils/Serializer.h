#pragma once

#include"../core/PasswordFile.h"
#include"../core/Password.h"
#include"../encrypt/Cipher.h"
#include"../encrypt/CipherRegistry.h"
#include"../utils/exceptions.h"
#include<stdexcept>

namespace utils {
  class Serializer {
  public:
    static clib::darray<core::PasswordEntry> deserializePasswords (const clib::Text& raw) {
      return deserializePasswords(raw.split('\n'));
    }

    static clib::darray<core::PasswordEntry> deserializePasswords (
        const clib::darray<clib::Text>& lines
    ) {

      clib::darray<core::PasswordEntry> l;
      for(std::size_t i = 0; i < lines.size(); i ++) {

        if(lines[i].empty()) continue;

        auto line = lines[i].split('\t');

        if(line.empty()) continue;

        if(line.size() < 4) 
          throw std::invalid_argument("File possibly corrupted, line format is invalid!");

        clib::darray<clib::Text> cipherArguments;
        for(std::size_t j = 4; j < line.size(); j ++) {
          cipherArguments.add(line[j]);
        }

        auto cipher = encrypt::CipherRegistry::get()
          .byType(line[3])->fromArgs(cipherArguments);

        l.add(core::PasswordEntry(
          line[0], line[1], line[2],
          cipher
        ));
      }

      return l;
    }
    static clib::Text serializePasswords (const clib::darray<core::PasswordEntry>& data) {
      clib::Text result;
      std::size_t passCount = data.size();
      for(std::size_t i = 0; i < passCount; i ++) {
        result += data[i].serialize();
        result += "\n";
      }
      return result;
    }
    static clib::Text serializeCipher (const encrypt::Cipher* c) {
      return c->type() + "\t" + c->serialize();
    }

    static encrypt::Cipher* deserializeCipher (const clib::Text& raw) {
      clib::darray<clib::Text> words = raw.split('\t');
      if(words.size() == 0)
        throw utils::EncryptionError("File possibly corrupted");

      clib::Text type = words[0];
      clib::darray<clib::Text> args;
      for(std::size_t i = 1; i < words.size(); i ++) {
        args.add(words[i]);
      }
      return encrypt::CipherRegistry::get()
        .byType(type)->fromArgs(args);
    }

  };
}
