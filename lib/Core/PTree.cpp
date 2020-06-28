//===-- PTree.cpp ---------------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ExecutionState.h"
#include "PTree.h"

using namespace klee;

PTree::PTree(ExecutionState *initialState) {
  nodes.resize(1024);
  allocated.resize(1024);
  nodes[1].state = initialState;
  initialState->ptreeNodeID = 1;
  allocated.set(1);
  allocated.set(0); // first entry is dummy
}


std::uint32_t PTree::insert(std::uint32_t parentNodeID, ExecutionState * state) {
  // resize when full
  if (allocated.all()) {
    const auto current_size = allocated.size();
    nodes.resize(current_size + 1024);
    allocated.resize(current_size + 1024);
  }

  // find free slot close to nodeID (simplified: just find next or start from beginning)
  int idx = allocated.find_next_unset(parentNodeID);
  if (idx == -1) idx = allocated.find_first_unset_in(2, parentNodeID);
  assert(idx != -1);

  allocated.set(idx);
  nodes[idx] = {.left = 0, .right = 0, .parent = parentNodeID, .state = state};
  state->ptreeNodeID = idx;
  // llvm::errs() << "new node at: " << idx << " (p:" << parentNodeID << ") for state: " << state->getID() << '\n';

  return idx;
}


void PTree::attach(std::uint32_t nodeID, ExecutionState * leftState, ExecutionState * rightState) {
  nodes[nodeID].state = nullptr;
  nodes[nodeID].left  = insert(nodeID, leftState);
  nodes[nodeID].right = insert(nodeID, rightState);
}

void PTree::remove(std::uint32_t nodeID) {
  do {
    auto parentNodeID = nodes[nodeID].parent;
    if (parentNodeID) {
      if (nodeID == nodes[parentNodeID].left) {
        nodes[parentNodeID].left = 0;
      } else {
        nodes[parentNodeID].right = 0;
      }
    }

    // llvm::errs() << "remove node at: " << nodeID << " (p:" << parentNodeID << ") for state: " << nodes[nodeID].state << '\n';
    allocated.reset(nodeID);
    nodeID = parentNodeID;
  } while (nodeID && !nodes[nodeID].left && !nodes[nodeID].right);
}

/*
void PTree::dump(llvm::raw_ostream &os) {
  ExprPPrinter *pp = ExprPPrinter::create(os);
  pp->setNewline("\\l");
  os << "digraph G {\n";
  os << "\tsize=\"10,7.5\";\n";
  os << "\tratio=fill;\n";
  os << "\trotate=90;\n";
  os << "\tcenter = \"true\";\n";
  os << "\tnode [style=\"filled\",width=.1,height=.1,fontname=\"Terminus\"]\n";
  os << "\tedge [arrowsize=.3]\n";
  std::vector<const PTreeNode*> stack;
  stack.push_back(root.get());
  while (!stack.empty()) {
    const PTreeNode *n = stack.back();
    stack.pop_back();
    os << "\tn" << n << " [shape=diamond";
    if (n->state)
      os << ",fillcolor=green";
    os << "];\n";
    if (n->left) {
      os << "\tn" << n << " -> n" << n->left.get() << ";\n";
      stack.push_back(n->left.get());
    }
    if (n->right) {
      os << "\tn" << n << " -> n" << n->right.get() << ";\n";
      stack.push_back(n->right.get());
    }
  }
  os << "}\n";
  delete pp;
}

PTreeNode::PTreeNode(PTreeNode *parent, ExecutionState *state) : parent{parent}, state{state} {
  state->ptreeNode = this;
}
*/
