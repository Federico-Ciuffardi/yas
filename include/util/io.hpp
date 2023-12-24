#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;
using std::filesystem::path;

using std::cin;
using std::cout;
using std::string;

inline string readStdio(const string &prompt = "", const string &def = "") {
  string s;
  cout << prompt;
  if (!def.empty()) {
    cout << "[" << def << "]";
  }
  cout << ":";

  if (def.empty() || cin.peek() != '\n') {
    cin >> s;
  }

  if (s.empty()) { // use default when non interactive
    s = def;
  }
  return s;
}

inline string readFile(const path &p) {
  ifstream in(p, ios::in | ios::binary);
  if (in) {
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

inline void writeYaml(const YAML::Node &yaml, const path &p) {
  create_directory(p.parent_path());
  ofstream out(p, ios::out);
  out << yaml;
  out.close();
}
