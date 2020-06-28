//===-- PTree.h -------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_PTREE_H
#define KLEE_PTREE_H

#include "klee/Expr/Expr.h"

#include "llvm/ADT/BitVector.h"

#include <vector>

namespace klee {
  class ExecutionState;

  class PTreeNode {
  public:
    std::uint32_t left {0};
    std::uint32_t right {0};
    std::uint32_t parent {0};
    ExecutionState *state {nullptr};
  };

  class PTree {
    std::vector<PTreeNode> nodes;
    llvm::BitVector allocated;

    std::uint32_t insert(std::uint32_t nodeID, ExecutionState * state);
  public:
    explicit PTree(ExecutionState *initialState);
    ~PTree() = default;

    void attach(std::uint32_t nodeID, ExecutionState *leftState, ExecutionState *rightState);
    void remove(std::uint32_t nodeID);
    const PTreeNode * getNode(std::uint32_t nodeID) const { return &nodes[nodeID]; }
    constexpr static std::uint32_t getRootID() { return 1; };
    // void dump(llvm::raw_ostream &os);
  };
}

#endif /* KLEE_PTREE_H */
