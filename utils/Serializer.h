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
      return deserializePasswords(raw.split('\n'));
    }

    static clib::List<core::PasswordEntry> deserializePasswords (
        const clib::List<clib::String> lines
    ) {

      std::cout << "IN DES PASSWORDS" << std::endl;
      std::cout << "LINE COUNT: " << lines.size() << std::endl;
      clib::List<core::PasswordEntry> l;
      for(std::size_t i = 0; i < lines.size(); i ++) {
        std::cout << " LINE " << (i+1) << std::endl;
        std::cout << lines[i] << std::endl;
        std::cout << "PASSED PRINT" << std::endl;

        if(lines[i].empty()) {  std::cout << "LEAVING 1" << std:: endl; continue; };

        auto line = lines[i].split(' ');

        if(line.empty()) { std::cout << "LEAVING 2" << std:: endl; continue; };

        if(line.size() < 4) 
          throw std::invalid_argument("File possibly corrupted, line format is invalid!");

        std::cout << "Getting into execution" << std::endl;

        clib::List<clib::String> cipherArguments;
        for(std::size_t j = 4; j < line.size(); j ++) {
          cipherArguments.add(line[j]);
        }

        std::cout << "GENERATING CIPHER NOW" << std::endl;
        auto cipher = encrypt::CipherFactory::create(
          stringToCipherType(line[3]), cipherArguments
        );
        std::cout << "CIPHER DONE" << std::endl;

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
        result +=
          data[i].getWebsite() + " " + data[i].getUsername() + " " + data[i].getPasswordEncrypted()
            + " " + serializeCipher(data[i].getCipher()) + "\n";
      }
      return result;
    }
    static clib::String serializeCipher (const encrypt::Cypher* c) {
      return cipherTypeToString(c->type()) + " " + c->serialize();
    }

    static encrypt::Cypher* deserializeCipher (const clib::String& raw) {
      clib::List<clib::String> words = raw.split(' ');
      if(words.size() == 0)
        throw utils::InvalidCipherTypeException("File possibly corrupted");
      encrypt::CipherType type = stringToCipherType(words[0]);
      clib::List<clib::String> args;
      std::cout << "IN DES CIPHER" << std::endl;
      std::cout << words[0] << std::endl;
      for(std::size_t i = 1; i < words.size(); i ++) {
        std::cout << words[i] << std::endl;
        args.add(words[i]);
      }
      std::cout << "END DES CIPHER" << std::endl;
      return encrypt::CipherFactory::create(type, args);
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
      std::cout << "CONVERTING " << s << " " <<  (s == cipherTypeToString(encrypt::CipherType::TestCipher)) << " " << s.size() << " " << cipherTypeToString(encrypt::CipherType::TestCipher).size() <<  std::endl;
      for(std::size_t i = 0; i < s.size(); i ++) {
        std::cout << "\"" << (unsigned)s[i] << "\" ";
      }
      std::cout << std::endl;

      std::cout << "GETTING IN THE IFS" << std::endl;
      if(s == cipherTypeToString(encrypt::CipherType::TestCipher)){
        std::cout << "1ST COMPARE DONE - RETURNING" << std::endl;
        return encrypt::CipherType::TestCipher;
      }
      std::cout << "1ST COMPARE DONE" << std::endl;
      if(s == cipherTypeToString(encrypt::CipherType::CaesarCipher)) {
        std::cout << "2ST COMPARE DONE - RETURNING" << std::endl;
        return encrypt::CipherType::CaesarCipher;
      }
      std::cout << "2ST COMPARE DONE" << std::endl;

      throw utils::InvalidCipherTypeException("INVALID CIPHER");
    }
  };
}
