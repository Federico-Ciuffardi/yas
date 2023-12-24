#include <command/Clone.hpp>
#include <command/Init.hpp>

#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>
#include <tclap/YasStdOutput.h>

#include <filesystem>
#include <iostream>
#include <string>

const std::string version     = "0.0.0";
const std::string description = "Yet Another Syncer";

void parse_add_args(std::vector<std::string> &commandArgs) {
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
  // TODO
}

void parse_clone_args(std::vector<std::string> &commandArgs) {
  // declare commands
  TCLAP::CmdLine cmd(description, ' ', version);

  /// url
  TCLAP::UnlabeledValueArg<std::string> argUrl(
      "url", "The url of a yas compatible git repository", true, "", "git url");
  cmd.add(argUrl);

  // parse
  cmd.parse(commandArgs);

  // build request
  Clone clone;
  clone.u = url(argUrl.getValue().c_str());
  clone.execute();
}

void parse_init_args(std::vector<std::string> &commandArgs) {
  // declare commands
  TCLAP::CmdLine cmd(description, ' ', version);

  /// synto
  TCLAP::ValueArg<fs::path> argSyncto("s", "syncto", "Path to sync files",
                                      false, "", "path");

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

int main(int argc, char **argv) {
  try {
    // declare
    TCLAP::CmdLine cmd(description, ' ', version);

    TCLAP::YasStdOutput yasStdOutput;
    cmd.setOutput(&yasStdOutput);

    /// command
    const std::vector<std::string>        commands = {"add", "clone", "init"};
    TCLAP::ValuesConstraint<std::string>  commandsConstraint(commands);
    TCLAP::IgnoreRestVisitor              v; // stop parsing at the command
    TCLAP::UnlabeledValueArg<std::string> argCommand(
        "command", "Command", true, "", &commandsConstraint, false, &v);
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
    std::vector<std::string> command_args = argCommandArgs.getValue();
    command_args.insert(command_args.begin(), 1, "yas " + command);
    TCLAP::Arg::endIgnoring();

    if (command == "add") {
      parse_add_args(command_args);
    } else if (command == "clone") {
      parse_clone_args(command_args);
    } else if (command == "init") {
      parse_init_args(command_args);
    }
  } catch (TCLAP::ArgException &arg_exception) {
    std::cerr << "error: " << arg_exception.error() << " for arg "
              << arg_exception.argId() << std::endl;
  }
}
