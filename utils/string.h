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

    long long toInt() const;
    static String fromInt(long long i);

    String& insert(std::size_t at, char c);
    String& trim(char c);

    List<String> split(char c) const;

    bool empty() const;

    void clear();
    std::size_t size() const;

    const char* raw() const;

  private:
    List<char> l;
  };

  inline String& String::trim(char c) {
    l.pop();

    while(!l.empty() && l[0] == c) {
      l.remove(0);
    }

    while(!l.empty() && *l.end() == c) {
      l.remove(l.size() - 1);
    }

    l.add('\0');
    return *this;
  }

  inline void String::clear() {
    l.clear();
    l.add('\0');
  }
  inline std::size_t String::size() const {
    return l.size() - 1;
  }

  inline String::String() { l.add('\0'); };
  inline String::String(const char* s): String(s, std::strlen(s)) { };
  inline String::String(const char* s, std::size_t len) {
    l = List<char>(s, len);
    l.add('\0');
  }

  inline bool String::empty() const {
    return size() == 0;
  }

  inline String& String::operator=(const char* s) {
    l = List<char>(s, std::strlen(s));
    l.add('\0');
    return *this;
  }
  inline char& String::operator[](std::size_t index) {
    return l[index];
  }

  inline const char& String::operator[](std::size_t index) const{
    return l[index];
  }
  inline String& String::operator+=(const String& s) {
    if(this == &s) {
      String copy = s;
      l.pop();
      l.extend(copy.l);
      return *this;
    }
    l.pop();
    l.extend(s.l);
    return *this;
  }
  inline String& String::operator+=(char c) {
    l.pop();
    l.add(c);
    l.add('\0');
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

  inline long long String::toInt() const {
    if(size() == 0 || (size() == 1 && l[0] == '-'))
      throw std::invalid_argument("Not an integer");
    
    signed sign = 1;
    std::size_t i = 0;

    if(l[0] == '-') {
      i = 1;
      sign = -1;
    }

    long long r = 0;
    for(; i < size(); i ++) {
      if(l[i] < '0' || l[i] > '9')
        throw std::invalid_argument("Not an integer");

      r = r * 10 + (l[i] - '0');
    }

    return r * sign;
  }

  inline String String::fromInt(long long i) {
    if(i == 0) return "0";

    String r;
    bool neg = i < 0;
    if(neg) i = -i;

    while(i > 0) {
      char c = (i % 10) + '0';
      r.insert(0, c);
      i /= 10;
    }

    if(neg) r.insert(0, '-');

    return r;
  }

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

  inline String& String::insert(std::size_t at, char c) {
    l.insert(at, c);
    return *this;
  }

  inline std::ostream& operator<<(std::ostream& stream, const String& s) {
    for(std::size_t i = 0; i < s.size(); i ++) {
      stream << s[i];
    }
    return stream;
  }

  inline bool operator==(const char* sraw, const String& s) {
    return s == sraw;
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

  inline const char* String::raw() const{
    return l.size() ? &l[0] : "";
  }
  

}

