#pragma once

#include"../utils/mystring.h"
#include"../utils/darray.h"
#include"./Command.h"

#include"../core/PasswordManager.h"

namespace gui {
  
  using TokenList = clib::darray<clib::Text>;

  class Shell {
  private:

    bool running;
    core::PasswordManager pm;
    
    clib::darray<Command> commands;

    bool promptConfirm();
    clib::Text promptForPassword();
  public:
    Shell();

    void execute(clib::Text cmd, const TokenList& t);

    bool isRunning() const;
    
    void stop();

    void print(const clib::Text& s) const;
    void print_line(clib::Text s) const;
    std::istream& in();
  };
}
