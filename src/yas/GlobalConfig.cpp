#include "GlobalConfig.h"

GlobalConfig &GlobalConfig::getInstance() {
  static GlobalConfig instance;
  return instance;
}

GlobalConfig::GlobalConfig() {
  const path HOME     = getenv("HOME");
  const path EXE_NAME = "yas";

  _dataPath  = HOME / ".local/share" / EXE_NAME;
  _reposPath = _dataPath / "repos";

  _configPath = HOME / ".config" / EXE_NAME;
}
