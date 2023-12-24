/* Reusable wrapper for libgit2 */

#pragma once

#include <git/GitRemote.hpp>
#include <util/url.hpp>

#include <git2.h>

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class GitRepo {

  // variable
private:
  git_repository *repo = NULL;

  // functions
public:
  // open
  GitRepo(const fs::path &p);
  // clone
  GitRepo(const nonstd::url &u, const fs::path &p);
  // add
  void add(const std::vector<fs::path> &paths);
  void addAll();
  // commit
  void commit(const std::string &message, const std::string &name,
              const std::string &email);
  // push
  void push(const std::vector<std::string> &refspecs = {"refs/heads/master"});
  // destructor
  ~GitRepo();

private:
  GitRepo();
};
