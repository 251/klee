//===-- Dot.cpp -------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Dot.h"

#include "klee/Core/TerminationTypes.h"

#include <cmath>
#include <iostream>
#include <sstream>


std::string terminationTypeColour(StateTerminationType type) {
  // Exit
  if (type == StateTerminationType::Exit)
    return "green";

  // Early
  if ((StateTerminationType::Exit < type && type <= StateTerminationType::MARK_EARLY) ||
      (StateTerminationType::MARK_EXECERR < type && type <= StateTerminationType::MARK_END)) {
    return "orange";
  }

  // Program error
  if (StateTerminationType::MARK_EARLY < type && type <= StateTerminationType::MARK_PROGERR)
    return "red";

  // User error
  if (StateTerminationType::MARK_PROGERR < type && type <= StateTerminationType::MARK_USERERR)
    return "blue";

  // Execution error
  if (StateTerminationType::MARK_USERERR < type && type <= StateTerminationType::MARK_EXECERR)
    return "darkblue";

  return "darkgrey";
}


void printDOT(const Tree &tree) {
  const auto &nodes = tree.nodes;

  // print header
  // - style defaults to intermediate nodes
  std::cout << "strict digraph PTree {\n"
               "node[shape=point,width=0.15,color=darkgrey];\n"
               "edge[color=darkgrey];\n\n";

  // print nodes
  // - change colour for leaf nodes
  // - attach branch and location info as tooltips
  std::string attribute;
  for (size_t i = 1; i < nodes.size(); ++i) {
    const auto &node = nodes[i];
    std::cout << 'N' << i << '[';
    bool isLeafNode = !node.left;
    std::stringstream ss(attribute);
    // node id, state id, asm line
    ss << "tooltip=\"node: " << i << "\\nstate: " << node.stateID << "\\nasm: " << node.asmLine;
    if (isLeafNode) {
      // termination type
      const auto terminationType = static_cast<StateTerminationType>(std::log2(node.terminationTypeMask));
      ss << "\\n" << terminationTypeName[static_cast<std::uint32_t>(terminationType)];
      const auto colour = terminationTypeColour(terminationType);
      ss << "\",color=" << colour;
    } else {
      // branch type
      ss << "\\n" << branchTypeName[static_cast<std::uint8_t>(node.branchType)] << '"';
    }
    std::cout << ss.str() << "];\n";
  }
  std::cout << '\n';

  // print edges
  for (size_t i = 1; i < nodes.size(); ++i) {
    if (nodes[i].left)
      std::cout << 'N' << i << "->{N" << nodes[i].left << " N" << nodes[i].right << "};\n";
  }

  // print footer
  std::cout << '}' << std::endl;
}
