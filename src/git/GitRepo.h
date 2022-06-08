/* Reusable wrapper for libgit2 */

#pragma once

#include <string>
#include <util/url.h>
#include <git2.h>
#include <filesystem>
#include <vector>
#include <git/GitRemote.h>

using std::filesystem::path;
using std::vector;

class GitRepo {

// variable 
private:
  git_repository* repo  = NULL;

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
