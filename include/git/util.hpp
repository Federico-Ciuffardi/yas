#pragma once

#include <util/io.hpp>

#include <git2.h>

#include <string>

namespace fs = std::filesystem;

// callback to get credentials
inline int credentialsCB(git_cred **out, [[maybe_unused]] const char *url,
                         const char                   *username_from_url,
                         [[maybe_unused]] unsigned int allowed_types,
                         [[maybe_unused]] void        *payload) {

  // TODO do not harcode ($HOME/.ssh/config)
  // TODO ssh passphrase
  // TODO https
  std::string identityFile = fs::path(getenv("HOME")) / ".ssh/id_ed25519";
  std::string pubkey       = readFile(identityFile + ".pub");
  std::string privkey      = readFile(identityFile);

  return git_credential_ssh_key_memory_new(out, username_from_url,
                                           pubkey.c_str(), privkey.c_str(), "");
}
