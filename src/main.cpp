#include <command/Clone.hpp>
#include <command/Init.hpp>

#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>
#include <tclap/YasStdOutput.h>

#include <filesystem>
#include <iostream>
#include <string>

const std::string version      = "0.0.0";
const std::string description  = "Yet Another Syncer";
const std::string command_name = "yas";

void parse_add_args(std::vector<std::string> &subcommand_args) {
  // declare subcommand
  TCLAP::CmdLine cmd(description, ' ', version);

  /// file paths
  TCLAP::UnlabeledMultiArg<std::string> argFiles("files", "Files to add", false,
                                                 "file paths");
  cmd.add(argFiles);

  // parse
  cmd.parse(subcommand_args);

  /// get files
  std::vector<std::string> files = argFiles.getValue();

  // build request
  // TODO
}

void parse_clone_args(std::vector<std::string> &subcommand_args) {
  // declare subcommand
  TCLAP::CmdLine cmd(description, ' ', version);

  /// url
  TCLAP::UnlabeledValueArg<std::string> argUrl(
      "url", "The url of a yas compatible git repository", true, "", "git url");
  cmd.add(argUrl);

  // parse
  cmd.parse(subcommand_args);

  // build request
  Clone clone;
  clone.u = nonstd::url(argUrl.getValue().c_str());
  clone.execute();
}

void parse_init_args(std::vector<std::string> &subcommand_args) {
  // declare subcommand
  TCLAP::CmdLine cmd(description, ' ', version);

  /// synto
  TCLAP::ValueArg<fs::path> arg_sync_to("s", "sync_to", "Path to sync files",
                                      false, "", "path");

  cmd.add(arg_sync_to);

  /// url
  TCLAP::UnlabeledValueArg<std::string> argUrl(
      "url", "The url of an empty git repository", true, "", "git url");
  cmd.add(argUrl);

  // parse
  cmd.parse(subcommand_args);

  // build request
  Init init;
  init.set_args({argUrl.getValue().c_str(), arg_sync_to.getValue()});
  init.execute();
}

std::vector<std::string> parse_main_args(int argc, char **argv) {
  // declare
  TCLAP::CmdLine cmd(description, ' ', version);

  TCLAP::YasStdOutput yasStdOutput;
  cmd.setOutput(&yasStdOutput);

  /// subcommand
  const std::vector<std::string>        subcommands = {"add", "clone", "init"};
  TCLAP::ValuesConstraint<std::string>  subcommands_constraint(subcommands);
  TCLAP::IgnoreRestVisitor              v; // stop parsing at the subcommand
  TCLAP::UnlabeledValueArg<std::string> arg_subcommand(
      "subcommand", "Subcommand", true, "", &subcommands_constraint, false, &v);
  cmd.add(arg_subcommand);

  /// subcommand args
  TCLAP::UnlabeledMultiArg<std::string> arg_subcommand_args(
      "subcommand_args", "Args used in subcommands.", false, "subcommand args");
  cmd.add(arg_subcommand_args);

  // parse
  cmd.parse(argc, argv);

  /// get subcommand
  std::string sub_command = arg_subcommand.getValue();

  /// get subcommand_args
  std::vector<std::string> subcommand_args = arg_subcommand_args.getValue();
  subcommand_args.insert(subcommand_args.begin(), 1,
                         command_name + " " + sub_command);
  TCLAP::Arg::endIgnoring();

  return subcommand_args;
}

int main(int argc, char **argv) {
  try {
    std::vector<std::string> subcommand_args = parse_main_args(argc, argv);
    const std::string        subcommand =
        subcommand_args[0].substr(command_name.size() + 1);
    if (subcommand == "add") {
      parse_add_args(subcommand_args);
    } else if (subcommand == "clone") {
      parse_clone_args(subcommand_args);
    } else if (subcommand == "init") {
      parse_init_args(subcommand_args);
    }
  } catch (TCLAP::ArgException &arg_exception) {
    std::cerr << "error: " << arg_exception.error() << " for arg "
              << arg_exception.argId() << std::endl;
  }
}
