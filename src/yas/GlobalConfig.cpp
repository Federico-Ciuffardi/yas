#include <yas/GlobalConfig.hpp>

GlobalConfig &GlobalConfig::getInstance() {
  static GlobalConfig instance;
  return instance;
}

GlobalConfig::GlobalConfig() {
  const path HOME     = getenv("HOME");
  const path EXE_NAME = "yas";

  _paths.dataDir   = HOME / ".local/share" / EXE_NAME;
  _paths.reposDir  = _paths.dataDir / "repos";
  _paths.configDir = HOME / ".config" / EXE_NAME;
}
