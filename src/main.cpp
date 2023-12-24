#include <command/Clone.hpp>
#include <command/Init.hpp>

#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>
#include <tclap/YasStdOutput.h>

#include <filesystem>
#include <iostream>
#include <string>

const static std::string version = "0.0.0";

const static std::string description = "Yet Another Syncer";

const static std::vector<std::string> commands = {"add", "clone", "init"};

int main(int argc, char **argv) {
  try {
    // declare
    TCLAP::CmdLine cmd(description, ' ', version);

    TCLAP::YasStdOutput yasStdOutput;
    cmd.setOutput(&yasStdOutput);

    /// command
    TCLAP::ValuesConstraint<std::string>  commandsConstraint(commands);
    TCLAP::IgnoreRestVisitor         v;
    TCLAP::UnlabeledValueArg<std::string> argCommand("command", "Command", true, "",
                                         &commandsConstraint, false, &v);
    cmd.add(argCommand);

    /// command args
    TCLAP::UnlabeledMultiArg<std::string> argCommandArgs(
        "command_args", "Commands used in the args.", false, "command args");
    cmd.add(argCommandArgs);

    // parse
    cmd.parse(argc, argv);

    /// get command
    std::string command = argCommand.getValue();

    /// get commandArgs
    std::vector<std::string> commandArgs = argCommandArgs.getValue();
    commandArgs.insert(commandArgs.begin(), 1,
                       "yas " + command); // TODO improve
    TCLAP::Arg::endIgnoring();                   // needed to parse again

    if (command == "add") {
      // declare commands
      TCLAP::CmdLine cmd(description, ' ', version);

      /// file paths
      TCLAP::UnlabeledMultiArg<std::string> argFiles("files", "Files to add", false,
                                         "file paths");
      cmd.add(argFiles);

      // parse
      cmd.parse(commandArgs);

      /// get files
      std::vector<std::string> files = argFiles.getValue();

      // build request
    } else if (command == "clone") {
      // declare commands
      TCLAP::CmdLine cmd(description, ' ', version);

      /// url
      TCLAP::UnlabeledValueArg<std::string> argUrl(
          "url", "The url of a yas compatible git repository", true, "",
          "git url");
      cmd.add(argUrl);

      // parse
      cmd.parse(commandArgs);

      // build request
      Clone clone;
      clone.u = url(argUrl.getValue().c_str());
      clone.execute();
    } else if (command == "init") {
      // declare commands
      TCLAP::CmdLine cmd(description, ' ', version);

      /// synto
      TCLAP::ValueArg<fs::path> argSyncto("s", "syncto", "Path to sync files", false, "",
                               "path");

      cmd.add(argSyncto);

      /// url
      TCLAP::UnlabeledValueArg<std::string> argUrl(
          "url", "The url of an empty git repository", true, "", "git url");
      cmd.add(argUrl);

      // parse
      cmd.parse(commandArgs);

      // build request
      Init init;
      init.u      = url(argUrl.getValue().c_str());
      init.syncto = argSyncto.getValue();
      init.execute();
    }
  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }
}
