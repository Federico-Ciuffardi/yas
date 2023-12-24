/* adds task specific stuff */

#pragma once

#include <git/GitRepo.hpp>
#include <util/url.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

class YasRepo {
  // inner structs
  struct Paths {
    fs::path gitDir;
    fs::path configDir;

    fs::path repoConfigFile;

    Paths(const std::string &name);
  };

  // const
public:
  const std::string name;
  const Paths       paths;
  fs::path          sync_to;

  // variable
private:
  std::unique_ptr<GitRepo> gitRepo;

  // functions
public:
  YasRepo(const fs::path &path);
  YasRepo(const nonstd::url &url);
  fs::path getLocal();
  void     init(fs::path);

private:
  YasRepo();
};
