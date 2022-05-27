#pragma once

#include <Command/Command.h>
#include <util/url.h>
#include <string>

using std::string;

class Clone : public Command {

// variable 
public:
  url u;

// functions
public:
  virtual void execute();
};
