#include "YasRepo.h"

#include <iostream>


using std::make_unique;

inline string nameFromUrl(url u){
  string name = u;
  name = name.substr(name.find_last_of('/')+1);
  name = name.substr(0,name.find('.'));
  return name;
}

// poner todos los params restantes (repo_file_dir, etc)
YasRepo::YasRepo(url u) { 
  gitRepo = make_unique<GitRepo>(u, REPOS_DIR.string()+"/"+nameFromUrl(u)); 
}

YasRepo::YasRepo(path p){
  gitRepo = make_unique<GitRepo>(p); // el paht se infiere de la url
}
