#pragma once

#include"../utils/string.h"
#include"../utils/array.h"

#include"../core/PasswordManager.h"

namespace gui {
  
  using TokenList = clib::List<clib::String>;

  class Shell {
  private:
    core::PasswordManager pm;

    bool promptConfirm();
    
    static const clib::String commandUsage(const clib::String& cmd);
  public:
    Shell();

    void open(const TokenList& t);
    void create(const TokenList& t);
    void close();
    void save_file();
    void save(const TokenList& t);
    void load(const TokenList& t);
    void update(const TokenList& t);
    void entryDelete(const TokenList& t);
    void list() const;
    void help() const;
    void listCiphers() const;

    void print(const clib::String& s) const;
    void print_line(clib::String s, char eol = '\n') const;
    std::istream& in();
  };
}
