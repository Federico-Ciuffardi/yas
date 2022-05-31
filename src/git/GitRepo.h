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
  GitRepo(path p);
  // clone
  GitRepo(url u, path p);
  // add
  void add(vector<path>);
  void addAll();
  // commit
  void commit(string message, string name, string email);
  // push
  void push(vector<string> refspecs = {"refs/heads/master"});
  // destructor 
  ~GitRepo();

private:
  GitRepo();
};
