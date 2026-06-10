#pragma once

#include"../utils/mystring.h"

namespace gui {

  class Shell;

  struct Command {
    clib::Text command;
    clib::Text usage;
    clib::Text description;
    void (*call)(Command& cmd, Shell& shell, const clib::darray<clib::Text>& t);

    Command(
      const clib::Text& command,
      const clib::Text& usage,
      const clib::Text& description,
      void (*call)(gui::Command& cmd, Shell& shell, const clib::darray<clib::Text>& t)
    ) : command(command), usage(usage), description(description), call(call) { };
  };
}
