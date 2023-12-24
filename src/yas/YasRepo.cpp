#include <yas/YasRepo.hpp>

#include <yas/GlobalConfig.hpp>

#include <util/io.hpp>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

inline std::string nameFromUrl(const url &u) {
  std::string name = u;
  name             = name.substr(name.find_last_of('/') + 1);
  name             = name.substr(0, name.find('.'));
  return name;
}

inline std::string nameFromPath(const fs::path &p) {
  std::string name = p;
  name             = name.substr(name.find_last_of('/') + 1);
  return name;
}

//////////////////
// YasRepoPaths //
//////////////////
YasRepo::Paths::Paths(const std::string &name) {
  GlobalConfig &gc = GlobalConfig::getInstance();

  gitDir    = gc.paths.reposDir / name;
  configDir = gitDir / ".yas";

  repoConfigFile = configDir / "config";
}

/////////////
// YasRepo //
/////////////

// poner todos los params restantes (repo_file_dir, etc)
YasRepo::YasRepo(const url &u) : name(nameFromUrl(u)), paths(name) {
  gitRepo = make_unique<GitRepo>(u, paths.gitDir);
}

YasRepo::YasRepo(const fs::path &p) : name(nameFromPath(p)), paths(name) {
  gitRepo = std::make_unique<GitRepo>(p);
}

// init
void YasRepo::init(fs::path _syncto) {
  if (exists(paths.repoConfigFile)) {
    std::cerr << "Could not init: the repo is already initialized" << std::endl;
    exit(1);
  }

  /// create necesary stuff (mainly mount point)
  YAML::Node yml;

  //// mount
  if (_syncto.empty()) {
    syncto = readStdio("syncto", "$HOME");
  } else {
    syncto = _syncto;
  }
  yml["syncto"] = syncto.string();

  //// write
  writeYaml(yml, paths.repoConfigFile);

  /// add all
  gitRepo->add({"*"});

  /// commit
  gitRepo->commit("Yas initialization commit", "yas", "yas@fake.email");

  /// push
  gitRepo->push();
}
