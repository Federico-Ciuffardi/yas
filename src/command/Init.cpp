#include <command/Init.hpp>

#include <yas/YasRepo.hpp>

#include <git/GitRemote.hpp>

#include <iostream>

void Init::execute() {
  // validate repo before initialize
  /// is empty?
  GitRemote gitRemote(m_args.url, GIT_DIRECTION_FETCH);
  if (!gitRemote.ls().empty()) {
    std::cerr
        << "Could not init yas repository:" << std::endl
        << "The provided git repo is not empty, please provide a empty repo"
        << std::endl;
    exit(1);
  }

  // clone
  YasRepo yasRepo(m_args.url);

  // init
  yasRepo.init(m_args.sync_to);
}

void Init::set_args(const Args &args) {
  m_args = args;
};
