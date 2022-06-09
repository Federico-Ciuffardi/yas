#pragma once

#include "Command.h"

#include <util/url.h>

class Init : public Command {

  // variable
public:
  url u;

  // functions
public:
  virtual void execute();
};
