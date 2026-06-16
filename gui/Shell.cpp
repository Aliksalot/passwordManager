#include"Shell.h"
#include"Command.h"

#include"../utils/mystring.h"
#include"../utils/darray.h"
#include"../core/Password.h"
#include"../core/PasswordView.h"
#include"../encrypt/CipherRegistry.h"

#include<iostream>


namespace gui {

  void Shell::clearScreen() const {
    print("\033[2J\033[H");
  }

  template<typename T>
  void Shell::paginate(
      const clib::darray<T>& items,
      void (*printItem)(Shell&, const T&)
  ) {

    const unsigned total = items.size();
    const unsigned pageCount = (total + PAGINATION_ITEMS_PER_PAGE - 1)
                                / PAGINATION_ITEMS_PER_PAGE;
    unsigned currentPage = 0;

    while(total > 0) {
      
      std::size_t from = currentPage * PAGINATION_ITEMS_PER_PAGE;
      std::size_t to = from + PAGINATION_ITEMS_PER_PAGE;
      if(to > total) to = total;

      print_line("--------------------");
      for(std::size_t i = from; i < to; i ++) {
        printItem(*this, items[i]);
      }
      print_line("--------------------");

      print_line(clib::Text("Current Page: ") + (currentPage + 1) + "/" + pageCount
          + " | Total entries: " + total);

      if(pageCount == 1) return;
      print_line("(n)ext (q)uit (p)revious");
      
      clib::Text input;
      clib::getLine(in(), input);
      if(input[0] == 'n' && currentPage < pageCount - 1) currentPage ++;
      if(input[0] == 'p' && currentPage > 0) currentPage --;
      if(input[0] == 'q') break;
    }
  }

  bool Shell::isRunning() const {
    return running;
  }

  void Shell::stop() {
    running = false;
  }

  Shell::Shell() {

    running = true;

    commands.add(Command(
      "open",
      "open <filename> <password>",
      "Decrypts the file at <filename> with <password> and loads it, allowing you to modify its content.",
      [](Command& cmd, Shell& shell, const clib::darray<clib::Text>& t) {
        if(t.size() < 2){
          shell.print_line("Invalid usage -> " + cmd.usage);
          return;
        }
        
        if(shell.pm.hasOpenFile()) {
          shell.print_line("You already have an open file!");
          return;
        }

        try{
          shell.pm.openFile(t[0], t[1]);
          shell.print_line("File opened successfully.");
        }catch(const std::exception& e) {
          shell.print_line(e.what());
        }catch(...) {
          shell.print_line("Unknown error occured, while trying to open file");
        }
      }
    ));

    commands.add(Command(
      "create",
      "create <filename> <cipher> <password>",
      "Creates a new file at <filename>. <cipher> will be used as a default cipher to encrypt the password entries. <password> is used to protect the file.",
      [](Command& cmd, Shell& shell, const clib::darray<clib::Text>& t) {
        if(t.size() != 3) {
          shell.print_line("Needs exactly 3 arguments: Invalid usage -> " + cmd.usage);
          return;
        }
        
        try{
          
          encrypt::Cipher* defaultCipher = encrypt::CipherRegistry::get()
            .byType(t[1])->fromShell(shell);

          shell.pm.createFile(t[0], defaultCipher, t[2]);

          shell.print_line("File created successfully!");

        }catch(const utils::InvalidCipherTypeError& e) {
          shell.print_line(e.what());
          shell.print_line("You must chose from one of the predefined cipher types: ");
          for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
            shell.print_line(ct);
          }
        };
      }
    ));

    commands.add(Command(
      "save",
      "save <website> <username> [<cipher>] <password>",
      "Creates a new password entry. If you don't provide [<cipher>] the default one will be used.",
      [](Command& cmd, Shell& shell, const clib::darray<clib::Text>& t) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }
        try{
          if(t.size() <= 2) {
            shell.print_line("Too few arguments: Invalid usage -> " + cmd.usage);
          }else if(t.size() == 3) {
            shell.pm.newPassword(t[0], t[1], t[2]);
            shell.print_line("Added new entry!");
          }else if(t.size() == 4) {
            encrypt::Cipher* cipher = encrypt::CipherRegistry::get()
              .byType(t[2])->fromShell(shell);

            if(!shell.pm.newPassword(t[0], t[1], t[3], cipher)) {
              shell.print_line("Couldn't add new password entry. ");
            }else {
              shell.print_line("Added new entry!");
            }
          }else {
            shell.print_line("Too many arguments: Invalid usage -> " + cmd.usage);
          }
        }catch(const utils::InvalidCipherTypeError& e) {
          shell.print_line(e.what());
          shell.print_line("You must chose from one of the predefined cipher types: ");
          for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
            shell.print_line(ct);
          }
        }
      }
    ));

    commands.add(Command(
      "load",
      "load <website> [<username>]",
      "Prints on screen all password entries that match your query with passwords DECRYPTED.",
      [](Command& cmd, Shell& shell, const clib::darray<clib::Text>& t) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }

        clib::darray<const core::PasswordEntry*> matches;
        if(t.size() < 1) {
          shell.print_line("Too few arguments: Invalid usage -> " + cmd.usage);
        }else if(t.size() == 1) {
          matches = shell.pm.loadPassword(t[0]);
        }else if(t.size() == 2) {
          matches = shell.pm.loadPassword(t[0], t[1]);
        }else {
          shell.print_line("Too many arguments: Invalid usage -> " + cmd.usage);
        }

        shell.paginate<const core::PasswordEntry*>
        (matches, [](Shell& shell, const core::PasswordEntry* const& entry) {
          shell.print_line(
              entry->getUsername() +
                "\t" + entry->getCipher()->decrypt(entry->getPasswordEncrypted()
          ));
        });
      }
    ));

    commands.add(Command(
      "update",
      "update <website> <username> <new-password>",
      "Updates entry.",
      [](Command& cmd, Shell& shell, const clib::darray<clib::Text>& t) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }

        if(t.size() != 3) {
          shell.print_line("Expected exactly 3 arguments: Invalid usage -> " + cmd.usage);
          return;
        }

        try{
          shell.pm.updatePassword(t[0], t[1], t[2]);
          
          shell.print_line("Password successfully updated!");
        }catch(const utils::PasswordModifyError& e) {
          shell.print_line(e.what());
          return;
        }
      }
    ));

    commands.add(Command(
      "delete",
      "delete <website> [<username>]",
      "Deletes one or multiple entries. If you don't provide a [<username>], you will be prompted to delete all entries from <website>.",
      [](Command& cmd, Shell& shell, const clib::darray<clib::Text>& t) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }

        if(t.size() < 1) {
          shell.print_line("Too few arguments: Invalid usage -> " + cmd.usage);
        }else if(t.size() == 1) {
          const clib::Text& website = t[0];
          shell.print_line("Warning - you are about to delete all passwords for website " + website + ". Are you sure?");
          if(shell.promptConfirm()) {
            shell.pm.deletePassword(website);
            shell.print_line("Delete successfull!");
          }
        }else if(t.size() == 2) {
          shell.pm.deletePassword(t[0], t[1]);
          shell.print_line("Delete successfull!");
        }else{
          shell.print_line("Too many arguments: Invalid usage -> " + cmd.usage);
        }
      }
    ));

    commands.add(Command(
      "list",
      "list",
      "Prints on screen all entries WITHOUT sensitive data (only website and username).",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }

        shell.paginate<const core::PasswordView*>
        (shell.pm.list(), [](Shell& shell, const core::PasswordView* const& entry) {
          shell.print_line(entry->getWebsite() + "\t" + entry->getUsername());
        });
      }
    ));

    commands.add(Command(
      "close",
      "close",
      "Closes current file. If it's dirty, you will be prompted.",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }
        if(shell.pm.isFileDirty()) {
          shell.print_line("File has unsaved changes. Do you wish to save before leaving?");
          if(shell.promptConfirm()) {
            clib::Text password = shell.promptForPassword();
            if(password.empty()) {
              shell.print_line("Password can not be empty!"); return;
            }
            shell.pm.saveFile(password);
          }
        }
        shell.pm.closeFile();
      }
    ));

    commands.add(Command(
      "exit",
      "exit",
      "Exits the program. If current file is dirty, you will be prompted to save it.",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        if(!shell.pm.hasOpenFile()) {
          shell.stop();
          return;
        }
        if(shell.pm.isFileDirty()) {
          shell.print_line("File has unsaved changes. Do you wish to save before leaving?");
          if(shell.promptConfirm()) {
            clib::Text password = shell.promptForPassword();
            if(password.empty()) {
              shell.print_line("Password can not be empty!"); return;
            }
            shell.pm.saveFile(password);
          }
        }
        shell.pm.closeFile();
        shell.stop();
      }
    ));

    commands.add(Command(
      "help",
      "help",
      "How did we get here?",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        for(auto& cmd: shell.commands) {
          shell.print_line(cmd.usage + " - " + cmd.description);
        }
      }
    ));

    commands.add(Command(
      "save_file",
      "save_file",
      "Saves the currently opened file.",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        if(!shell.pm.hasOpenFile()) {
          shell.print_line("No file open");
          return;
        }
        clib::Text password = shell.promptForPassword();
        if(password.empty()) {
          shell.print_line("Password can not be empty!"); return;
        }
        shell.pm.saveFile(password);
        shell.print_line("File saved successfully!");
      }
    ));
    commands.add(Command(
      "ciphers",
      "ciphers",
      "Provides a list of all implemented ciphers.",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        for(auto& ct: encrypt::CipherRegistry::get().cipherTypes()) {
          shell.print_line(ct);
        }
      }
    ));

    commands.add(Command(
      "clear",
      "clear",
      "Clears the screen",
      [](Command&, Shell& shell, const clib::darray<clib::Text>&) {
        shell.clearScreen();
      })
    );
  }

  bool Shell::promptConfirm() {
    clib::Text w;
    do{
      print_line("(y)es or (n)o");
      in() >> w;
      if(w[0] == 'y')
        return true;
      if(w[0] == 'n')
        return false;
    }while(true);
  }

  clib::Text Shell::promptForPassword() {
    print_line("Enter this file's password: ");
    clib::Text password;
    in() >> password;
    return password;
  }

  std::istream& Shell::in() {
    std::cout << "pm < ";
    if(pm.hasOpenFile()) {
      std::cout << pm.getFilePath() << " < ";
    }
    return std::cin;
  }

  void Shell::print(const clib::Text& s) const {
    std::cout << "pm > ";
    if(pm.hasOpenFile()) {
      std::cout << pm.getFilePath() << " > ";
    }
    std::cout << s;
  }
  void Shell::print_line(clib::Text s) const {
    print(s += '\n');
  }

  void Shell::execute(clib::Text s, const TokenList& tokens) {

    if(s.trim(' ').empty()) {
      return;
    }

    if(!running) {
      throw std::runtime_error("Shell has already been exited. ");
    }

    for(Command& cmd: commands) {
      if(cmd.command == s) {
        cmd.call(cmd, *this, tokens);
        return;
      }
    }

    print_line("Invalid command. Check \"help\" for a list of commands");
  }

}
