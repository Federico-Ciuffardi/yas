/* adds task specific stuff */

#pragma once

#include <git/GitRepo.h>
#include <string>
#include <util/url.h>
#include <memory>
#include <filesystem>

using std::string;
using std::unique_ptr;
using std::filesystem::path;

struct YasRepoPaths{
  path syncDir;
  path gitDir;
  path configsDir;

  path mntFile;
  path repoConfigFile;

  YasRepoPaths(const string &name);
};

class YasRepo {
// const
public:
  const string name;
  const YasRepoPaths paths;

// variable 
private:
  unique_ptr<GitRepo> gitRepo;

// functions
public:
  YasRepo(const path &p);
  YasRepo(const url &u);
  path getLocal();
  void init();

private:
  YasRepo();
};
