#pragma once

#include"../utils/string.h"

namespace gui {

  class Shell;

  struct Command {
    clib::String command;
    clib::String usage;
    clib::String description;
    void (*call)(Command& cmd, Shell& shell, const clib::List<clib::String>& t);

    Command(
      const clib::String& command,
      const clib::String& usage,
      const clib::String& description,
      void (*call)(gui::Command& cmd, Shell& shell, const clib::List<clib::String>& t)
    ) : command(command), usage(usage), description(description), call(call) { };
  };
}
