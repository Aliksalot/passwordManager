#pragma once

#include"./array.h"
#include<cstddef>
#include<stdexcept>
#include<cstring>
#include<ostream>
#include<istream>

namespace clib{

  class String {
  public:
    String();
    String(const char* s);
    String(const char* s, std::size_t len);

    const char& operator[](std::size_t index) const;
    char& operator[](std::size_t index);
    String& operator+=(const String& s);
    String& operator+=(char c);
    String& operator+=(const char* s);
    String& operator=(const char* s);
    bool operator==(const String& s) const;
    bool operator==(const char* s) const;
    bool operator!=(const String& s) const;
    bool operator!=(const char* s) const;

    List<String> split(char c) const;

    bool empty() const;

    void clear();
    std::size_t size() const;

  private:
    List<char> l;
  };

  inline void String::clear() {
    l.clear();
  }
  inline std::size_t String::size() const {
    return l.size();
  }

  inline String::String() { };
  inline String::String(const char* s): String(s, std::strlen(s)) { };
  inline String::String(const char* s, std::size_t len) {
    l = List<char>(s, len);
  }

  inline bool String::empty() const {
    return l.empty();
  }

  inline String& String::operator=(const char* s) {
    l = List<char>(s, std::strlen(s));
    return *this;
  }
  inline char& String::operator[](std::size_t index) {
    return l[index];
  }

  inline const char& String::operator[](std::size_t index) const{
    return l[index];
  }
  inline String& String::operator+=(const String& s) {
    l.extend(s.l);
    return *this;
  }
  inline String& String::operator+=(char c) {
    l.add(c);
    return *this;
  }
  inline String& String::operator+=(const char* s) {
    return *this += String(s);
  }
  inline String operator+(const String& a, const String& b) {
    String result = a;
    return result += b;
  }
  inline bool String::operator==(const String& s) const {
    if(size() != s.size()) return false;

    for(std::size_t i = 0; i < size(); i ++) {
      if(l[i] != s[i]) return false;
    }
    return true;
  }
  inline bool String::operator==(const char* s) const {
    return *this == String(s);
  }

  inline bool String::operator!=(const String& s) const {
    return !(*this == s);
  }
  inline bool String::operator!=(const char* s) const {
    return !(*this == s);
  }

  //TODO pretty heavy, if time allows fix it
  inline List<String> String::split(char c) const {
    List<String> out;
    String temp;
    std::size_t stringLength = size();
    for(std::size_t i = 0; i < stringLength; i ++) {
      if(l[i] == c) {
        out.add(temp);
        temp = "";
        continue;
      }

      temp += l[i];
    }
    out.add(temp);
    return out;
  }
  inline std::ostream& operator<<(std::ostream& stream, const String& s) {
    for(std::size_t i = 0; i < s.size(); i ++) {
      stream << s[i];
    }
    return stream;
  }
  inline std::istream& operator>>(std::istream& stream, String& s) {
    s.clear();

    char c;
    while(stream.get(c)) {
      if(c == ' ' || c == '\n' || c == '\t') break;

      s += c;
    }

    return stream;
  }

}

