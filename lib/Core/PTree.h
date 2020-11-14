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

#include "ExecutionState.h"
#include "PTreeWriter.h"
#include "klee/Core/BranchTypes.h"
#include "klee/Expr/Expr.h"
#include "klee/Support/ErrorHandling.h"

#include "llvm/ADT/PointerIntPair.h"

namespace klee {
  class ExecutionState;
  class PTreeNode;
  /* PTreeNodePtr is used by the Random Path Searcher object to efficiently
  record which PTreeNode belongs to it. PTree is a global structure that
  captures all  states, whereas a Random Path Searcher might only care about
  a subset. The integer part of PTreeNodePtr is a bitmask (a "tag") of which
  Random Path Searchers PTreeNode belongs to. */
  constexpr int PtrBitCount = 3;
  using PTreeNodePtr = llvm::PointerIntPair<PTreeNode *, PtrBitCount, uint8_t>;

  enum class PTreeNodeMode {
    NONE = 0,
    NeedsUpdate = 1U,
  };

  class PTreeNode {
  public:
    PTreeNode *parent {nullptr};
    PTreeNodePtr left;
    PTreeNodePtr right;
    ExecutionState *state {nullptr};
    std::uint64_t id {0};
    std::uint32_t terminationTypeMask {0};
    std::uint32_t asmLine {0};
    BranchType branchType {BranchType::UNKNOWN};
    std::uint8_t mode {0};

    PTreeNode(const PTreeNode&) = delete;
    PTreeNode(PTreeNode *parent, ExecutionState *state, std::uint64_t id);
    ~PTreeNode() = default;
  };

  class PTree {
    std::unique_ptr<PTreeWriter> writer;
    /// Unique ID for next new child node
    std::int64_t nextID {1};
    /// Number of registered user IDs
    std::uint8_t registeredIds {0};

  public:
    PTreeNodePtr root;
    PTree(ExecutionState *initialState, const std::string &dbPath);
    ~PTree() = default;

    /// Branch from PTreeNode node and attach states, convention: rightState is parent
    void attach(PTreeNode *node, ExecutionState *leftState,
                ExecutionState *rightState, BranchType reason);
    /// Remove node from tree
    void remove(PTreeNode *node);
    /// Dump process tree in .dot format into os (debug)
    void dump(llvm::raw_ostream &os) const;
    /// Get next ID for process tree subtree (up to 3 supported)
    std::uint8_t getNextId();
  };
}

#endif /* KLEE_PTREE_H */
