#pragma once

#include <command/Command.hpp>
#include <util/url.hpp>

#include <filesystem>

namespace fs = std::filesystem;

class Init : public Command {

  // variable
public:
  url      u;
  fs::path syncto;

  // functions
public:
  virtual void execute();
};
