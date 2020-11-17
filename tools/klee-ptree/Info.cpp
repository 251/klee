//===-- Info.cpp ------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Info.h"
#include "klee/Core/TerminationTypes.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <unordered_map>


void printBranchInfo(const Tree &tree) {
  std::vector<std::uint32_t> branchTypes(static_cast<std::uint8_t>(BranchType::MARK_END));
  std::unordered_map<std::uint32_t, std::uint32_t> locations;

  // count branch types and locations in tree
  for (size_t i = 1; i < tree.nodes.size(); ++i) {
    const auto &n = tree.nodes[i];
    // ignore leaf nodes
    if (n.left) {
      if (static_cast<std::uint8_t>(n.branchType) >= static_cast<std::uint8_t>(BranchType::MARK_END)) {
        std::cerr << "Illegal branch type value: " << static_cast<std::uint16_t>(n.branchType) << std::endl;
        exit(EXIT_FAILURE);
      }
      ++branchTypes[static_cast<std::uint8_t>(n.branchType)];
      ++locations[n.asmLine];
    }
  }

  // print branches
  std::cout << "# branch types\n";
  for (size_t i = 1; i < static_cast<std::uint8_t>(BranchType::MARK_END); ++i) {
    std::cout << branchTypeName[i] << ": " << branchTypes[i] << '\n';
  }
  std::cout << '\n';

  // print locations
  // - sort by count/line
  std::vector<std::pair<std::uint32_t,std::uint32_t>> sortedLocations;
  for (const auto &kv : locations)
    sortedLocations.emplace_back(kv.first, kv.second);
  std::sort(sortedLocations.begin(), sortedLocations.end(),
            [](const auto lhs, const auto rhs){
              return lhs.second > rhs.second || (lhs.second == rhs.second && lhs.first < rhs.first); });

  std::cout << "# branch locations (asm line: count)\n";
  for  (const auto kv : sortedLocations)
    std::cout << kv.first << ": " << kv.second << '\n';
}


void printTreeInfo(const Tree &tree) {
  std::vector<std::uint32_t> terminationTypes(static_cast<std::uint32_t>(StateTerminationType::MARK_END));
  std::vector<std::uint32_t> depths;

  // traverse tree
  std::vector<std::tuple<std::uint32_t, std::uint32_t>> stack; // (id, depth)
  stack.emplace_back(std::make_tuple(1, 0));

  std::uint32_t id {0};
  std::uint32_t depth {0};

  const auto &nodes = tree.nodes;
  while (!stack.empty()) {
    bool isLeaf = true;
    std::tie(id, depth) = stack.back();
    stack.pop_back();

    if (nodes[id].right) {
      stack.emplace_back(std::make_tuple(nodes[id].right, depth+1));
      isLeaf = false;
    }

    if (nodes[id].left) {
      stack.emplace_back(std::make_tuple(nodes[id].left, depth+1));
      isLeaf = false;
    }

    // set depth and termination type
    if (isLeaf) {
      if (depth >= depths.size())
        depths.resize(depth + 1, 0);

      ++depths[depth];

      const auto terminationType = std::log2(nodes[id].terminationTypeMask);
      if (terminationType >= terminationTypes.size()) {
        std::cerr << "Illegal termination type: " << terminationType
                  << " in node with id " << id << std::endl;
        exit(EXIT_FAILURE);
      }

      ++terminationTypes[terminationType];
    }
  }

  // print general info
  std::cout << "# general information:\n"
            << "leaf nodes: " << (nodes.size() / 2) << '\n'
            << "max. depth: " << (depths.size() - 1) << "\n\n";

  // print termination types (sparse)
  std::cout << "# termination types:\n";
  for (size_t i = 1; i < terminationTypes.size(); ++i) {
    if (terminationTypes[i])
      std::cout << terminationTypeName[i] << ": " << terminationTypes[i] << '\n';
  }
  std::cout << std::endl;

  // print depths (sparse)
  std::cout << "# depths (depth: count):\n";
  for (size_t i = 0; i < depths.size(); ++i) {
    if (depths[i])
      std::cout << i << ": "<< depths[i] << '\n';
  }
}
