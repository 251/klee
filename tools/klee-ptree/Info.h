//===-- Info.h --------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_INFO_H
#define KLEE_INFO_H

#include "Tree.h"


/// print branch types and locations
void printBranchInfo(const Tree &tree);

/// print tree/leaf node information
void printTreeInfo(const Tree &tree);

#endif
