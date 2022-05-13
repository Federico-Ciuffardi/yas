#include <algorithm>
#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>

using namespace std;
using namespace TCLAP;

const static string version = "0.0.0";

const static string description = "Yet Another Syncer";

const static vector<string> commands = {"add", "init"};

class MyOutput : public StdOutput {
public:
  inline void _shortUsage(CmdLineInterface &_cmd, std::ostream &os) const {
    std::list<Arg *> argList = _cmd.getArgList();
    std::string progName = _cmd.getProgramName();
    XorHandler xorHandler = _cmd.getXorHandler();
    std::vector<std::vector<Arg *>> xorList = xorHandler.getXorList();

    std::string s = progName + " ";

    // first the xor
    for (int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++) {
      s += " {";
      for (ArgVectorIterator it = xorList[i].begin(); it != xorList[i].end();
           it++)
        s += (*it)->shortID() + "|";

      s[s.length() - 1] = '}';
    }

    // then the rest
    for (ArgListIterator it = argList.begin(); it != argList.end(); it++)
      if (!xorHandler.contains((*it)))
        s += " " + (*it)->shortID();

    // if the program name is too long, then adjust the second line offset
    int secondLineOffset = static_cast<int>(progName.length()) + 2;
    if (secondLineOffset > 75 / 2)
      secondLineOffset = static_cast<int>(75 / 2);

    spacePrint(os, s, 75, 0, secondLineOffset);
  }

  virtual void usage(CmdLineInterface &_cmd) {
    std::cout << std::endl << "usage: ";

    _shortUsage(_cmd, std::cout);

    cout << "Concatenate FILE(s) to standard output.";

    std::cout << std::endl << std::endl << "Where: " << std::endl << std::endl;

    _longUsage(_cmd, std::cout);

    std::cout << std::endl;
  }
};

int main(int argc, char **argv) {
  try {
    TCLAP::CmdLine cmd(description, ' ', version);

    MyOutput my;
    cmd.setOutput(&my);

    ValuesConstraint<string> commandsConstraint(commands);
    IgnoreRestVisitor v;
    UnlabeledValueArg<string> argCommand("command", "Used command", true, "", &commandsConstraint, false, &v);
    cmd.add(argCommand);

    UnlabeledMultiArg<string> argCommandArgs(
        "command_args", "Commands used in the args", false, "command args");
    cmd.add(argCommandArgs);

    cmd.parse(argc, argv);

    std::string command = argCommand.getValue();
    vector<string> commandArgs = argCommandArgs.getValue();
    commandArgs.insert(commandArgs.begin(), 1, command);
    Arg::endIgnoring();

    if (command == "add") {
      TCLAP::CmdLine cmd(description, ' ', version);

      MyOutput my;
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
