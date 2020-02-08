#pragma once

#include "Solver.h"
#include "Vec.h"
#include <iostream>
#include <string>

namespace {

int help(std::string Invocation) {
  std::cout << "This utility program generates inequality system and dumps it "
               "to disk.\n";
  std::cout << "Usage: " << Invocation << " filename\n";
  return -1;
}

} // namespace

int main(int argc, char *argv[]) {
  if (argc < 2)
    return help(std::string(argv[0]));
  cimmpino::System Inequalities;
  Inequalities.serialize(argv[1]);
}
