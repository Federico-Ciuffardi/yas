#include <algorithm>
#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>
#include <yastclap/CustomOutput.h>

#include <command/Clone.h>
#include <command/Init.h>

using namespace std;
using namespace TCLAP;

const static string version = "0.0.0";

const static string description = "Yet Another Syncer";

const static vector<string> commands = {"add", "clone", "init"};

int main(int argc, char **argv) {
  try {
    // declare
    TCLAP::CmdLine cmd(description, ' ', version);

    CustomOutput customOutput;
    cmd.setOutput(&customOutput);

    /// command
    ValuesConstraint<string> commandsConstraint(commands);
    IgnoreRestVisitor v;
    UnlabeledValueArg<string> argCommand("command", "Command", true, "", &commandsConstraint, false, &v);
    cmd.add(argCommand);

    /// command args
    UnlabeledMultiArg<string> argCommandArgs(
        "command_args", "Commands used in the args.", false, "command args");
    cmd.add(argCommandArgs);

    // parse
    cmd.parse(argc, argv);

    /// get command
    std::string command = argCommand.getValue();

    /// get commandArgs
    vector<string> commandArgs = argCommandArgs.getValue();
    commandArgs.insert(commandArgs.begin(), 1, "yas "+command); // TODO improve
    Arg::endIgnoring(); // needed to parse again

    if (command == "add") {
      // declare commands
      TCLAP::CmdLine cmd(description, ' ', version);

      /// file paths
      UnlabeledMultiArg<string> argFiles(
          "files", "Files to add", false, "file paths");
      cmd.add(argFiles);

      // parse
      cmd.parse(commandArgs);

      /// get files
      vector<string> files = argFiles.getValue();

      // build request
    } else if (command == "clone") {
      // declare commands
      TCLAP::CmdLine cmd(description, ' ', version);

      /// url
      UnlabeledValueArg<string> argUrl("url", "The url of a yas compatible git repository", true,"", "git url");
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

      /// url
      UnlabeledValueArg<string> argUrl("url", "The url of an empty git repository", true,"", "git url");
      cmd.add(argUrl);

      // parse
      cmd.parse(commandArgs);

      // build request
      Init init;
      init.u = url(argUrl.getValue().c_str());
      init.execute();

    }
  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }
}

