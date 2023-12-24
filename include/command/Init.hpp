#pragma once

#include <command/Command.hpp>
#include <util/url.hpp>

#include <filesystem>

namespace fs = std::filesystem;

class Init : public Command {
public:
  struct Args {
    nonstd::url url;
    fs::path sync_to;
  };

  virtual void execute();
  void set_args(const Args&);

private:
  Args m_args;
};
