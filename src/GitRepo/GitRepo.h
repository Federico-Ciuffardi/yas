/* Reusable wrapper for libgit2 */

#pragma once

#include <string>
#include <util/url.h>
#include <git2.h>
#include <filesystem>

using std::string;
using std::filesystem::path;

class GitRepo {

// variable 
private:
  git_repository* raw = NULL;

// functions
public:
  GitRepo(path p);
  GitRepo(url u, path p);

  string getPath();
  string getRemoteUrl();

  string add();

private:
  GitRepo();
};
