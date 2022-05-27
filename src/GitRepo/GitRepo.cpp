#include "GitRepo.h"

#include <filesystem>
#include <iostream>
#include <util/file.h>

using std::filesystem::exists;
using std::filesystem::create_directory;

// callback to get credentials
int credentialsCB(git_cred **out, [[maybe_unused]] const char *url,
                  const char *username_from_url,
                  [[maybe_unused]] unsigned int allowed_types,
                  [[maybe_unused]] void *payload) {

  // TODO do not harcode ($HOME/.ssh/config)
  // TODO ssh passphrase
  // TODO https
  string identityFile = string(getenv("HOME")) + "/.ssh/id_ed25519";
  string pubkey = read(identityFile + ".pub");
  string privkey = read(identityFile);

  return git_credential_ssh_key_memory_new(out, username_from_url,
                                           pubkey.c_str(), privkey.c_str(), "");
}

// clone to path
GitRepo::GitRepo(url u, path p) {
  // create dir if not exists
  if(!exists(p)){
    create_directories(p);
  }

  // init libgit2 (probably first time interacting with git)
  git_libgit2_init();

  // set clone options
  git_clone_options options = GIT_CLONE_OPTIONS_INIT;
  /// handle auth
  options.fetch_opts.callbacks.credentials = credentialsCB;
  /// TODO handle progress

  // clone
  int error = git_clone(&raw, u.c_str(), p.c_str(), &options);
  if (error < 0) {
    fprintf(stderr, "Could not clone repository:\n  %s\n",
            git_error_last()->message);
    exit(1);
  }
}

// open
GitRepo::GitRepo(path p) {
  // init libgit2 (probably first time interacting with git)
  git_libgit2_init();

  // open
  int error = git_repository_open(&raw, p.c_str());
  if (error < 0) {
    fprintf(stderr, "Could not open repository:\n  %s\n",
            git_error_last()->message);
    exit(1);
  }
}
