#include "YasRepo.h"

#include <yas/GlobalConfig.h>

#include <iostream>
#include <util/io.h>
#include <filesystem>

using std::make_unique;
using std::filesystem::exists;
using std::endl;
using std::cerr;

inline string nameFromUrl(url u){
  string name = u;
  name = name.substr(name.find_last_of('/')+1);
  name = name.substr(0,name.find('.'));
  return name;
}

inline string nameFromPath(path p){
  string name = p;
  name = name.substr(name.find_last_of('/')+1);
  return name;
}

//////////////////
// YasRepoPaths //
//////////////////
YasRepoPaths::YasRepoPaths(string name){
  GlobalConfig& gc = GlobalConfig::getInstance();
  
  gitDir     = gc.reposPath / name;
  syncDir    = gitDir;
  configsDir = gitDir / ".yas";

  repoConfigFile = configsDir / "config";

}

/////////////
// YasRepo //
/////////////

// poner todos los params restantes (repo_file_dir, etc)
YasRepo::YasRepo(url u) : name(nameFromUrl(u)), paths(name) {
  gitRepo = make_unique<GitRepo>(u, paths.gitDir);
}

YasRepo::YasRepo(path p) : name(nameFromPath(p)), paths(name) {
  gitRepo = make_unique<GitRepo>(p);
}

// init
void YasRepo::init(){
  if(exists(paths.repoConfigFile)){
    cerr<<"Could not init: the repo is already initialized"<<endl;
    exit(1);
  }

  /// create necesary stuff (mainly mount point)
  YAML::Node  yml;

  //// mount
  yml["sync_point"] = readStdio("sync_point", "$HOME");

  //// write
  writeYaml(yml, paths.repoConfigFile);

  /// add all
  gitRepo->add({"*"});

  /// commit
  gitRepo->commit("Yas initialization commit", "yas", "yas@fake.email");

  /// push
  gitRepo->push();
}
