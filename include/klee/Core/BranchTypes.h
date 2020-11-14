//===-- BranchTypes.h -------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_BRANCHTYPES_H
#define KLEE_BRANCHTYPES_H

#include <cstdint>


///@brief Reason ExecutionState forked
enum class BranchType : std::uint8_t {
  UNKNOWN = 0,
  Alloc,
  Br,
  Call,
  Exact,
  Free,
  Getval,
  Memop,
  Realloc,
  Switch,
  IndirectBr,
  MARK_END,
};


///@brief String representations of BranchType
static char const * const branchTypeName[12] = {
  [(std::uint8_t)BranchType::UNKNOWN] = "UNKNOWN",
  [(std::uint8_t)BranchType::Alloc] = "Alloc",
  [(std::uint8_t)BranchType::Br] = "Br",
  [(std::uint8_t)BranchType::Call] = "Call",
  [(std::uint8_t)BranchType::Exact] = "Exact",
  [(std::uint8_t)BranchType::Free] = "Free",
  [(std::uint8_t)BranchType::Getval] = "Getval",
  [(std::uint8_t)BranchType::Memop] = "Memop",
  [(std::uint8_t)BranchType::Realloc] = "Realloc",
  [(std::uint8_t)BranchType::Switch] = "Switch",
  [(std::uint8_t)BranchType::IndirectBr] = "IndirectBr",
  [(std::uint8_t)BranchType::MARK_END] = "",
};

#endif
