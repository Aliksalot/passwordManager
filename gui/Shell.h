#pragma once

#include"../utils/string.h"
#include"../utils/array.h"
#include"./Command.h"

#include"../core/PasswordManager.h"

namespace gui {
  
  using TokenList = clib::List<clib::String>;

  class Shell {
  private:
    core::PasswordManager pm;
    
    clib::List<Command> commands;

    bool promptConfirm();
  public:
    Shell();

    bool execute(const clib::String& cmd, const TokenList& t);

    void print(const clib::String& s) const;
    void print_line(clib::String s, char eol = '\n') const;
    std::istream& in();
  };
}
