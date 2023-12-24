/* adds task specific stuff */

#pragma once

#include <git/GitRepo.hpp>
#include <util/url.hpp>

#include <filesystem>
#include <memory>
#include <string>

using std::string;
using std::unique_ptr;
using std::filesystem::path;

class YasRepo {
  // inner structs
  struct Paths {
    path gitDir;
    path configDir;

    path repoConfigFile;

    Paths(const string &name);
  };

  // const
public:
  const string name;
  const Paths  paths;
  path         syncto;

  // variable
private:
  unique_ptr<GitRepo> gitRepo;

  // functions
public:
  YasRepo(const path &p);
  YasRepo(const url &u);
  path getLocal();
  void init(path);

private:
  YasRepo();
};
