#pragma once

#include <util/io.hpp>

#include <git2.h>

#include <string>

using std::string;

// callback to get credentials
inline int credentialsCB(git_cred **out, [[maybe_unused]] const char *url,
                         const char                   *username_from_url,
                         [[maybe_unused]] unsigned int allowed_types,
                         [[maybe_unused]] void        *payload) {

  // TODO do not harcode ($HOME/.ssh/config)
  // TODO ssh passphrase
  // TODO https
  string identityFile = path(getenv("HOME")) / ".ssh/id_ed25519";
  string pubkey       = readFile(identityFile + ".pub");
  string privkey      = readFile(identityFile);

  return git_credential_ssh_key_memory_new(out, username_from_url,
                                           pubkey.c_str(), privkey.c_str(), "");
}
