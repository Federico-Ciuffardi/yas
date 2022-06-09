/* Reusable wrapper for libgit2 */

#pragma once

#include <filesystem>
#include <git/GitRemote.h>
#include <git2.h>
#include <string>
#include <util/url.h>
#include <vector>

using std::vector;
using std::filesystem::path;

class GitRepo {

  // variable
private:
  git_repository *repo = NULL;

  // functions
public:
  // open
  GitRepo(const path &p);
  // clone
  GitRepo(const url &u, const path &p);
  // add
  void add(const vector<path> &paths);
  void addAll();
  // commit
  void commit(const string &message, const string &name, const string &email);
  // push
  void push(const vector<string> &refspecs = {"refs/heads/master"});
  // destructor
  ~GitRepo();

private:
  GitRepo();
};
