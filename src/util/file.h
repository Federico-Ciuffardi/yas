#pragma once

#include <cstdio>
#include <fstream>
#include <string>
#include <filesystem>
#include <cerrno>

using std::string;
using std::ifstream;
using std::ios;
using std::filesystem::path;

inline string read(path p) {
  ifstream in(p, ios::in | ios::binary);
  if (in)
  {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
  }
  perror("error:");
  throw(errno);
}
