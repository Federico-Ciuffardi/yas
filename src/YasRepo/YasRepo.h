/* adds task specific stuff */

#pragma once

#include <GitRepo/GitRepo.h>
#include <util/url.h>
#include <memory>
#include <filesystem>

using std::unique_ptr;
using std::filesystem::path;

class YasRepo {
// const
private:
  const path HOME = string(getenv("HOME"));
  const path LOCAL_DIR = HOME.string()+"/.local/share/yas";
  const path REPOS_DIR = LOCAL_DIR.string()+"/repos";

// variable 
private:
  unique_ptr<GitRepo> gitRepo;


// functions
public:
  YasRepo(path p);
  YasRepo(url u);

private:
  YasRepo();
};
