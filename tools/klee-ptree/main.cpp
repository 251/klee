//===-- main.cpp ------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <cstdlib>
#include <iostream>

#include "Info.h"
#include "Dot.h"


void print_usage() {
  std::cout << "Usage: klee-ptree <option> /path/ptree.db\n\n"
               "Options:\n"
               "\tbranchinfo  -  print branch statistics\n"
               "\tdot         -  print tree in .dot format\n"
               "\ttreeinfo    -  print tree statistics"
               "\n";
}


int main(int argc, char * argv[]) {
  if (argc != 3) {
    print_usage();
    exit(1);
  }

  // parse options
  void (*action)(const Tree&);
  std::string option(argv[1]);
  if (option == "branchinfo") {
    action = printBranchInfo;
  } else if (option == "dot") {
    action = printDOT;
  } else if (option == "treeinfo") {
    action = printTreeInfo;
  } else {
    print_usage();
    exit(1);
  }

  // create tree
  std::string path(argv[2]);
  Tree tree(path);

  // print results
  action(tree);
}