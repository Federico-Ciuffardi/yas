#include <command/Init.hpp>

#include <yas/YasRepo.hpp>

#include <git/GitRemote.hpp>

#include <iostream>

void Init::execute() {
  // validate repo before initialize
  /// is empty?
  GitRemote gitRemote(u, GIT_DIRECTION_FETCH);
  if (!gitRemote.ls().empty()) {
      std::cerr << "Could not init yas repository:" << std::endl
         << "The provided git repo is not empty, please provide a empty repo"
         << std::endl;
    exit(1);
  }

  // clone
  YasRepo yasRepo(u);

  // init
  yasRepo.init(syncto);
}
