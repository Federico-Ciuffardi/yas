#pragma once

#include "Command.h"
#include <util/url.h>

class Clone : public Command {

// variable 
public:
  url u;

// functions
public:
  virtual void execute();
};
