#include <git/GitRemote.hpp>

#include <git/util.hpp>

#include <git2/net.h>

#include <iostream>

using std::cerr;
using std::endl;

// open
GitRemote::GitRemote(const url &u, const git_direction &dir) {
  // init libgit2 (probably first time interacting with git)
  git_libgit2_init();

  // create
  int error = git_remote_create_detached(&remote, u.c_str());
  if (error < 0) {
    cerr << "Could not create detached remote:" << endl
         << git_error_last()->message << endl;
    exit(1);
  }

  // connect
  git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
  callbacks.credentials          = credentialsCB;
  error = git_remote_connect(remote, dir, &callbacks, NULL, NULL);
  if (error < 0) {
    cerr << "Could not connect to detached remote:" << endl
         << git_error_last()->message << endl;
    exit(1);
  }
}

GitRemote::GitRemote(git_repository *repo, const git_direction &dir) {
  // create
  int error = git_remote_lookup(&remote, repo, "origin");
  if (error < 0) {
    cerr << "Could not lookup remote:" << endl
         << git_error_last()->message << endl;
    exit(1);
  }

  // connect
  git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
  callbacks.credentials          = credentialsCB;
  error = git_remote_connect(remote, dir, &callbacks, NULL, NULL);
  if (error < 0) {
    cerr << "Could not connect to remote:" << endl
         << git_error_last()->message << endl;
    exit(1);
  }
}

vector<const git_remote_head *> GitRemote::ls() {
  // git_remote_ls
  size_t                  size;
  const git_remote_head **headsArr;
  int                     error = git_remote_ls(&headsArr, &size, remote);
  if (error < 0) {
    cerr << "Could not ls:" << endl << git_error_last()->message << endl;
    exit(1);
  }

  // to vector
  vector<const git_remote_head *> headsVec(size);
  for (size_t i = 0; i < size; i++) {
    headsVec[i] = headsArr[i];
  }

  return headsVec;
}

GitRemote::~GitRemote() {
  git_remote_free(remote);
}
