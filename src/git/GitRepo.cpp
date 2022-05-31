#include "GitRepo.h"

#include <git/util.h>
#include <git2/types.h>
#include <util/functional.h>
#include <iostream>

using std::endl;
using std::cerr;
using std::filesystem::create_directories;

//////////////////
// Constructors //
//////////////////

// clone to path
GitRepo::GitRepo(url u, path p) {
  // create dir if not exists
  create_directories(p);

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
    cerr<<"Could not clone git repository:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }
}

// open
GitRepo::GitRepo(path p) {
  // init libgit2 (probably first time interacting with git)
  git_libgit2_init();

  // open
  int error = git_repository_open(&repo, p.c_str());
  if (error < 0) {
    cerr<<"Could not open git repository:"<<endl<<git_error_last()->message<<endl;
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
void GitRepo::add(vector<path> paths){
  // load index
  git_index* index = NULL;
  int error = git_repository_index(&index,repo);
  if (error < 0) {
    cerr<<"Could not get the repository index:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // from `vector<path>` to `vector<char*>`
  function<char*(const path&)> toCharArr = ([](const path &p){ return (char*) p.c_str(); });
  vector<char*> pathsCharArr = mapf(paths, toCharArr);

  // add to index
  const git_strarray pathspec = {&pathsCharArr[0], pathsCharArr.size()};
  error = git_index_add_all(index, &pathspec, GIT_INDEX_ADD_DEFAULT, NULL, NULL);
  if (error < 0) {
    cerr<<"Could not add to index:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // write index
  error = git_index_write(index);
  if (error < 0) {
    cerr<<"Could not write index:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // free index
  git_index_free(index);
}

void GitRepo::addAll(){
  add({"*"});
}

// commit
void GitRepo::commit(string message, string name, string email){
  // load index
  git_index* index = NULL;
  int error = git_repository_index(&index,repo);
  if (error < 0) {
    cerr<<"Could not get the repository index:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // write index into tree and get that tree
  git_oid tree_oid;
  error = git_index_write_tree(&tree_oid, index);
  if (error < 0) {
    cerr<<"Could not write tree:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }
  git_tree *tree;
	error = git_tree_lookup(&tree, repo, &tree_oid);
  if (error < 0) {
    cerr<<"Could not get tree:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // prettify message
  git_buf buffer;
	error = git_message_prettify(&buffer, message.c_str(), 0, '#');
  if (error < 0) {
    cerr<<"Could not prettify message:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // create signature 
	git_signature *signature;
  error = git_signature_now(&signature, name.c_str(), email.c_str());
  if (error < 0) {
    cerr<<"Could not create signature:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // commit
  git_oid commit_oid;
  git_commit_create_v(&commit_oid, repo, "HEAD", signature, signature, NULL,
                      buffer.ptr, tree, 0);
  if (error < 0) {
    cerr<<"Could not create commit:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }

  // free
  git_buf_free(&buffer);
  git_signature_free(signature);
  git_tree_free(tree);
  git_index_free(index);
}

void GitRepo::push(vector<string> refspecs){

  // create remote
  GitRemote remote(repo, GIT_DIRECTION_PUSH);

  // refspec
  function<char*(const string&)> toCharArr = ([](const string &s){ return (char*) s.c_str(); });
  vector<char*> refspecsCharArr = mapf(refspecs, toCharArr);
  const git_strarray refspecsStrarray = { &refspecsCharArr[0], refspecsCharArr.size()};

  // push
  int error = git_remote_push(remote.remote, &refspecsStrarray, NULL);
  if (error < 0) {
    cerr<<"Could not push to remote:"<<endl<<git_error_last()->message<<endl;
    exit(1);
  }
}
