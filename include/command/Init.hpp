#pragma once

#include <command/Command.hpp>
#include <util/url.hpp>

#include <filesystem>

using std::filesystem::path;

class Init : public Command {

  // variable
public:
  url  u;
  path syncto;

  // functions
public:
  virtual void execute();
};
