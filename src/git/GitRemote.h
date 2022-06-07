/* Reusable wrapper for libgit2 */

#pragma once

#include <util/url.h>
#include <git2.h>
#include <vector>

using std::vector;

class GitRepo; // forward declaration

class GitRemote {

// variable 
private:
  git_remote* remote = NULL;

// functions
public:
  GitRemote(url u, git_direction dir);
  vector<const git_remote_head*> ls();
  ~GitRemote();

private:
  GitRemote(git_repository *repo, git_direction dir);
  GitRemote();

friend GitRepo;
};
