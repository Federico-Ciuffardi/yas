#include <yas/GlobalConfig.hpp>

#include <filesystem>

namespace fs = std::filesystem;

GlobalConfig &GlobalConfig::getInstance() {
  static GlobalConfig instance;
  return instance;
}

GlobalConfig::GlobalConfig() {
  const fs::path HOME     = getenv("HOME");
  const fs::path EXE_NAME = "yas";

  _paths.dataDir   = HOME / ".local/share" / EXE_NAME;
  _paths.reposDir  = _paths.dataDir / "repos";
  _paths.configDir = HOME / ".config" / EXE_NAME;
}
