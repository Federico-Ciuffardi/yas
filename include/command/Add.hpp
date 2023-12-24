#pragma once

#include <command/Command.hpp>

class Add : public Command {
public:
  virtual void execute();
};
