#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class GlobalConfig {
  // inner structs
  struct Paths {
    fs::path dataDir;
    fs::path configDir;
    fs::path reposDir;
  };

  // singleton boilerplate
public:
  static GlobalConfig &getInstance();

private:
  GlobalConfig();

public:
  GlobalConfig(GlobalConfig const &)   = delete;
  void operator=(GlobalConfig const &) = delete;

  // configs
  /// public are read only (const ref to private)
  /// private are read/write (must be friend to write)
private:
  Paths _paths;

public:
  const Paths &paths = _paths;
};
