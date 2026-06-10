#pragma once

#include"../utils/string.h"
#include"../utils/array.h"
#include"./Command.h"

#include"../core/PasswordManager.h"

namespace gui {
  
  using TokenList = clib::List<clib::String>;

  class Shell {
  private:

    bool running;
    core::PasswordManager pm;
    
    clib::List<Command> commands;

    bool promptConfirm();
    clib::String promptForPassword();
  public:
    Shell();

    void execute(clib::String cmd, const TokenList& t);

    bool isRunning() const;
    
    void stop();

    void print(const clib::String& s) const;
    void print_line(clib::String s) const;
    std::istream& in();
  };
}
