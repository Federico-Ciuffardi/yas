/* Reusable wrapper for libgit2 */

#pragma once

#include <util/url.hpp>

#include <git2.h>

#include <vector>

class GitRepo; // forward declaration

class GitRemote {

  // variable
private:
  git_remote *remote = NULL;

  // functions
public:
  GitRemote(const nonstd::url &u, const git_direction &dir);
  std::vector<const git_remote_head *> ls();
  ~GitRemote();

private:
  GitRemote(git_repository *repo, const git_direction &dir);
  GitRemote();

  friend GitRepo;
};
