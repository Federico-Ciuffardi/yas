#pragma once

#include <command/Command.hpp>
#include <util/url.hpp>

class Clone : public Command {

  // variable
public:
  nonstd::url u;

  // functions
public:
  virtual void execute();
};
