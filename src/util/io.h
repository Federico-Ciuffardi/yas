#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>

using std::ifstream;
using std::ofstream;
using std::ios;
using std::string;
using std::filesystem::path;

using std::string;
using std::cin;
using std::cout;

inline string readStdio(const string &prompt = "", const string &def = "") {
  string s;
  cout << prompt;
  if (!def.empty()) {
    cout << "[" << def << "]";
  }
  cout << ":";

  if (!def.empty() && cin.peek() == '\n') {
    s = def;
  } else {
    cin >> s;
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
  out<<yaml;
  out.close();
}

