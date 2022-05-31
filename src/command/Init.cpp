#include "Init.h"

#include <iostream>
#include <yas/YasRepo.h>
#include <git/GitRemote.h>

using std::endl;
using std::cerr;

void Init::execute(){
  // validate repo before initialize
  /// is empty? 
  GitRemote gitRemote(u, GIT_DIRECTION_FETCH);
  if(!gitRemote.ls().empty()){
    cerr<<"Could not init yas repository:"<<endl<<"The provided git repo is not empty, please provide a empty repo"<<endl;
    exit(1);
  }

  // clone
  YasRepo yasRepo(u);

  // init
  yasRepo.init();
}

