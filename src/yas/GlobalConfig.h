#pragma once

#include <filesystem>
#include <string>

using std::string;
using std::filesystem::path;

class GlobalConfig {
  // singleton boilerplate
public:
  static GlobalConfig &getInstance();

private:
  GlobalConfig();

public:
  GlobalConfig(GlobalConfig const &) = delete;
  void operator=(GlobalConfig const &) = delete;

  // configs
  /// public are read only (const ref to private)
  /// private are read/write (must be friend to write)
private:
  path _dataPath;
  path _configPath;
  path _reposPath;

public:
  const path &dataPath   = _dataPath;
  const path &configPath = _configPath;
  const path &reposPath  = _reposPath;
};
