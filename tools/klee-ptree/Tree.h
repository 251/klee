//===-- Tree.h --------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_TREE_H
#define KLEE_TREE_H

#include "klee/Core/BranchTypes.h"

#include <string>
#include <vector>


/// A Tree node representing a PTreeNode
struct Node final {
  std::uint64_t left {0};
  std::uint64_t right {0};
  std::uint32_t stateID {0};
  std::uint32_t asmLine {0};
  BranchType branchType {BranchType::UNKNOWN};
  std::uint32_t terminationTypeMask {0};
};


/// An in-memory representation of a complete process tree
struct Tree final {
  /// sorted vector or Nodes
  std::vector<Node> nodes; // node IDs start with 1: skip first node in traversal

  /// Reads complete ptree.db into memory
  explicit Tree(const std::string &path);
  ~Tree() = default;
};

#endif
