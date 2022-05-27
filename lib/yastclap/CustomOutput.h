#include <tclap/StdOutput.h>

using namespace std;
using namespace TCLAP;

class CustomOutput : public StdOutput {

public:
  virtual void usage(CmdLineInterface &c);

  virtual void version(CmdLineInterface &c);

  virtual void failure(CmdLineInterface &c, ArgException &e);

protected:
  void _shortUsage(CmdLineInterface &c, ostream &os) const;

  void _longUsage(CmdLineInterface &c, ostream &os) const;

  void spacePrint(ostream &os, const string &s, int maxWidth, int indentSpaces,
                  int secondLineOffset) const;
};

inline void CustomOutput::version(CmdLineInterface &_cmd) {
  string progName = _cmd.getProgramName();
  string xversion = _cmd.getVersion();

  cout << endl << progName << "  version: " << xversion << endl << endl;
}

inline void CustomOutput::usage(CmdLineInterface &_cmd) {
  cout << endl << "USAGE: " << endl << endl;

  _shortUsage(_cmd, cout);

	spacePrint( std::cout, _cmd.getMessage(), 75, 3, 0 );

  cout << endl << endl << "Where: " << endl << endl;

  _longUsage(_cmd, cout);
}

inline void CustomOutput::failure(CmdLineInterface &_cmd, ArgException &e) {
  string progName = _cmd.getProgramName();

  cerr << "PARSE ERROR: " << e.argId() << endl
       << "             " << e.error() << endl
       << endl;

  if (_cmd.hasHelpAndVersion()) {
    cerr << "Brief USAGE: " << endl;

    _shortUsage(_cmd, cerr);

    cerr << endl
         << "For complete USAGE and HELP type: " << endl
         << "   " << progName << " " << Arg::nameStartString() << "help" << endl
         << endl;
  } else
    usage(_cmd);

  throw ExitException(1);
}

inline void CustomOutput::_shortUsage(CmdLineInterface &_cmd,
                                      ostream &os) const {
  list<Arg *> argList = _cmd.getArgList();
  string progName = _cmd.getProgramName();
  XorHandler xorHandler = _cmd.getXorHandler();
  vector<vector<Arg *>> xorList = xorHandler.getXorList();

  string s = progName + " ";

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

  spacePrint(os, s, 75, 3, secondLineOffset);
}

inline void CustomOutput::_longUsage(CmdLineInterface &_cmd,
                                     ostream &os) const {
  list<Arg *> argList = _cmd.getArgList();
  XorHandler xorHandler = _cmd.getXorHandler();
  vector<vector<Arg *>> xorList = xorHandler.getXorList();

  // first the xor
  for (int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++) {
    for (ArgVectorIterator it = xorList[i].begin(); it != xorList[i].end();
         it++) {
      spacePrint(os, (*it)->longID(), 75, 3, 3);
      spacePrint(os, (*it)->getDescription(), 75, 5, 0);

      if (it + 1 != xorList[i].end())
        spacePrint(os, "-- OR --", 75, 9, 0);
    }
    os << endl << endl;
  }

  // then the rest
  for (ArgListIterator it = argList.begin(); it != argList.end(); it++)
    if (!xorHandler.contains((*it))) {
      spacePrint(os, (*it)->longID(), 75, 3, 3);
      spacePrint(os, (*it)->getDescription(), 75, 5, 0);
      os << endl;
    }
}

inline void CustomOutput::spacePrint(ostream &os, const string &s, int maxWidth,
                                     int indentSpaces,
                                     int secondLineOffset) const {
  int len = static_cast<int>(s.length());

  if ((len + indentSpaces > maxWidth) && maxWidth > 0) {
    int allowedLen = maxWidth - indentSpaces;
    int start = 0;
    while (start < len) {
      // find the substring length
      // int stringLen = min<int>( len - start, allowedLen );
      // doing it this way to support a VisualC++ 2005 bug
      using namespace std;
      int stringLen = min<int>(len - start, allowedLen);

      // trim the length so it doesn't end in middle of a word
      if (stringLen == allowedLen)
        while (stringLen >= 0 && s[stringLen + start] != ' ' &&
               s[stringLen + start] != ',' && s[stringLen + start] != '|')
          stringLen--;

      // ok, the word is longer than the line, so just split
      // wherever the line ends
      if (stringLen <= 0)
        stringLen = allowedLen;

      // check for newlines
      for (int i = 0; i < stringLen; i++)
        if (s[start + i] == '\n')
          stringLen = i + 1;

      // print the indent
      for (int i = 0; i < indentSpaces; i++)
        os << " ";

      if (start == 0) {
        // handle second line offsets
        indentSpaces += secondLineOffset;

        // adjust allowed len
        allowedLen -= secondLineOffset;
      }

      os << s.substr(start, stringLen) << endl;

      // so we don't start a line with a space
      while (s[stringLen + start] == ' ' && start < len)
        start++;

      start += stringLen;
    }
  } else {
    for (int i = 0; i < indentSpaces; i++)
      os << " ";
    os << s << endl;
  }
}
