#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

inline std::string readStdio(const std::string &prompt = "", const std::string &def = "") {
  std::string s;
  std::cout << prompt;
  if (!def.empty()) {
    std::cout << "[" << def << "]";
  }
  std::cout << ":";

  if (def.empty() || std::cin.peek() != '\n') {
    std::cin >> s;
  }

  if (s.empty()) { // use default when non interactive
    s = def;
  }
  return s;
}

inline std::string readFile(const fs::path &p) {
  std::ifstream in(p, std::ios::in | std::ios::binary);
  if (in) {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
  }
  perror("error:");
  throw(errno);
}

inline void writeYaml(const YAML::Node &yaml, const fs::path &p) {
  create_directory(p.parent_path());
  std::ofstream out(p, std::ios::out);
  out << yaml;
  out.close();
}
