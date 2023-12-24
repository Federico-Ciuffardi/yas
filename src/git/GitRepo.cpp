#include <git/GitRepo.hpp>

#include <git/util.hpp>
#include <util/functional.hpp>

#include <git2/types.h>

#include <iostream>

namespace fs = std::filesystem;

//////////////////
// Constructors //
//////////////////

// clone to path
GitRepo::GitRepo(const url &u, const fs::path &p) {
  // create dir if not exists
  fs::create_directories(p);

  // init libgit2 (probably first time interacting with git)
  git_libgit2_init();

  // set clone options
  git_clone_options options = GIT_CLONE_OPTIONS_INIT;

  /// handle auth
  options.fetch_opts.callbacks.credentials = credentialsCB;

  /// TODO handle progress

  // execute clone
  int error = git_clone(&repo, u.c_str(), p.c_str(), &options);
  if (error < 0) {
    std::cerr << "Could not clone git repository:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }
}

// open
GitRepo::GitRepo(const fs::path &p) {
  // init libgit2 (probably first time interacting with git)
  git_libgit2_init();

  // open
  int error = git_repository_open(&repo, p.c_str());
  if (error < 0) {
    std::cerr << "Could not open git repository:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }
}

GitRepo::~GitRepo() {
  git_repository_free(repo);
}

//////////////
// Commands //
//////////////

// add
void GitRepo::add(const std::vector<fs::path> &paths) {
  // load index
  git_index *index = NULL;
  int        error = git_repository_index(&index, repo);
  if (error < 0) {
    std::cerr << "Could not get the repository index:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // from `vector<path>` to `vector<char*>`
  std::function<char *(const fs::path &)> toCharArr =
      ([](const fs::path &p) { return (char *)p.c_str(); });
  std::vector<char *> pathsCharArr = mapf(paths, toCharArr);

  // add to index
  const git_strarray pathspec = {&pathsCharArr[0], pathsCharArr.size()};
  error =
      git_index_add_all(index, &pathspec, GIT_INDEX_ADD_DEFAULT, NULL, NULL);
  if (error < 0) {
    std::cerr << "Could not add to index:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // write index
  error = git_index_write(index);
  if (error < 0) {
    std::cerr << "Could not write index:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // free index
  git_index_free(index);
}

void GitRepo::addAll() {
  add({"*"});
}

// commit
void GitRepo::commit(const std::string &message, const std::string &name,
                     const std::string &email) {
  // load index
  git_index *index = NULL;
  int        error = git_repository_index(&index, repo);
  if (error < 0) {
    std::cerr << "Could not get the repository index:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // write index into tree and get that tree
  git_oid tree_oid;
  error = git_index_write_tree(&tree_oid, index);
  if (error < 0) {
    std::cerr << "Could not write tree:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }
  git_tree *tree;
  error = git_tree_lookup(&tree, repo, &tree_oid);
  if (error < 0) {
    std::cerr << "Could not get tree:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // prettify message
  git_buf buffer;
  error = git_message_prettify(&buffer, message.c_str(), 0, '#');
  if (error < 0) {
    std::cerr << "Could not prettify message:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // create signature
  git_signature *signature;
  error = git_signature_now(&signature, name.c_str(), email.c_str());
  if (error < 0) {
    std::cerr << "Could not create signature:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // commit
  git_oid commit_oid;
  git_commit_create_v(&commit_oid, repo, "HEAD", signature, signature, NULL,
                      buffer.ptr, tree, 0);
  if (error < 0) {
    std::cerr << "Could not create commit:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }

  // free
  git_buf_free(&buffer);
  git_signature_free(signature);
  git_tree_free(tree);
  git_index_free(index);
}

void GitRepo::push(const std::vector<std::string> &refspecs) {

  // create remote
  GitRemote remote(repo, GIT_DIRECTION_PUSH);

  // refspec
  std::function<char *(const std::string &)> toCharArr =
      [](const std::string &s) { return (char *)s.c_str(); };
  std::vector<char *> refspecsCharArr  = mapf(refspecs, toCharArr);
  const git_strarray  refspecsStrarray = {&refspecsCharArr[0],
                                          refspecsCharArr.size()};

  // push
  int error = git_remote_push(remote.remote, &refspecsStrarray, NULL);
  if (error < 0) {
    std::cerr << "Could not push to remote:" << std::endl
              << git_error_last()->message << std::endl;
    exit(1);
  }
}
