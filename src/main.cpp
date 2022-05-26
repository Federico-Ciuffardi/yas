#include <algorithm>
#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>
#include <yastclap/CustomOutput.h>

using namespace std;
using namespace TCLAP;

const static string version = "0.0.0";

const static string description = "Yet Another Syncer";

const static vector<string> commands = {"add", "init"};

int main(int argc, char **argv) {
  try {
    TCLAP::CmdLine cmd(description, ' ', version);

    CustomOutput customOutput;
    cmd.setOutput(&customOutput);

    ValuesConstraint<string> commandsConstraint(commands);
    IgnoreRestVisitor v;
    UnlabeledValueArg<string> argCommand("command", "Command", true, "", &commandsConstraint, false, &v);
    cmd.add(argCommand);

    UnlabeledMultiArg<string> argCommandArgs(
        "command_args", "Commands used in the args.", false, "command args");
    cmd.add(argCommandArgs);

    cmd.parse(argc, argv);

    std::string command = argCommand.getValue();
    vector<string> commandArgs = argCommandArgs.getValue();
    commandArgs.insert(commandArgs.begin(), 1, command);
    Arg::endIgnoring();

    if (command == "add") {
      TCLAP::CmdLine cmd(description, ' ', version);

      CustomOutput my;
      cmd.setOutput(&my);

      UnlabeledMultiArg<string> argFiles(
          "files", "Files to add", false, "file paths");
      cmd.add(argFiles);

      cmd.parse(commandArgs);

      vector<string> files = argFiles.getValue();

      for(string file : files){
        cout<<file<<" ";
      }
      cout<<endl;

      cout << "add command" << endl;
    } else if (command == "init") {
      cout << "init command" << endl;
    }

  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }
}
