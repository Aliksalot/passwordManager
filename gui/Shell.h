#pragma once

#include"../utils/string.h"
#include"../utils/array.h"

#include"../core/PasswordManager.h"

namespace gui {
  
  using TokenList = clib::List<clib::String>;

  class Shell {
  private:
    core::PasswordManager pm;

    bool promptSave();

    static std::istream& in();
    static void print(const clib::String& s);
    static void print_line(const clib::String& s, char eol = '\n');
  public:
    Shell();
    void open(const TokenList& t);
    void create(const TokenList& t);
    void close();
    void save(const TokenList& t);
    void load(const TokenList& t);
    void update(const TokenList& t);
    void remove(const TokenList& t);
    void list(const TokenList& t);
    void exit(const TokenList& t);
    void help();
  };
}
