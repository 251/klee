//===-- TerminationTypes.h --------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_TERMINATIONTYPES_H
#define KLEE_TERMINATIONTYPES_H

#include <cstdint>


///@brief Categorises StateTerminationType.
enum class StateTerminationClass : std::uint8_t {
  NONE = 0, ///< (do not write tests)
  Exit,     ///< normal program exit
  Early,    ///< early termination
  ProgErr,  ///< program errors
  UserErr,  ///< user errors
  ExecErr,  ///< execution errors
};


///@brief Reason an ExecutionState got terminated.
enum class StateTerminationType : std::uint32_t {
  RUNNING               =  0U, ///< still running
  // normal exit
  Exit                  =  1U, ///< end of execution path
  // early terminations I
  Interrupted           =  2U, ///< execution reached timeout
  MaxDepth              =  3U, ///< max branch depth
  OutOfMemory           =  4U, ///< reached memory limit
  OutOfStackMemory      =  5U, ///< reached stack limit
  Replay                =  6U, ///< unseeded during replay
  MARK_EARLY            =  6U,
  // program errors
  Abort                 = 10U, ///< call to abort() or klee_abort()
  Assert                = 11U, ///< failed assertion
  BadVectorAccess       = 12U, ///< out-of-bounds vector access
  Free                  = 13U, ///< free() on stack/global variable
  Model                 = 14U, ///< allocation with (possibly huge) symbolic size
  Overflow              = 15U, ///< UBSAN: arithmetic overflow
  Ptr                   = 16U, ///< inaccessible memory range in klee_check_memory_access() test or invalid pointer
  ReadOnly              = 17U, ///< attempt to write read-only memory
  ReportError           = 18U, ///< user-defined error via klee_report_error()
  MARK_PROGERR          = 18U,
  // user errors
  User                  = 21U, ///< invalid arguments/config/data supplied by user
  MARK_USERERR          = 21U,
  // execution errors
  Execution             = 25U, ///< execution engine error (failed assumptions, unimplemented functionality)
  External              = 26U, ///< failed external call
  MARK_EXECERR          = 26U,
  // early terminations II (don't write tests)
  Merge                 = 28U, ///< merged state
  SilentExit            = 29U, ///< call to klee_silent_exit()
  Solver                = 30U, ///< solver error or timeout
  MARK_END,
};


///@brief String representations of StateTerminationType
static char const * const terminationTypeName[32] = {
  [(std::uint32_t)StateTerminationType::RUNNING] = "RUNNING",
  [(std::uint32_t)StateTerminationType::Exit] = "Exit",
  [(std::uint32_t)StateTerminationType::Interrupted] = "Interrupted",
  [(std::uint32_t)StateTerminationType::MaxDepth] = "MaxDepth",
  [(std::uint32_t)StateTerminationType::OutOfMemory] = "OutOfMemory",
  [(std::uint32_t)StateTerminationType::OutOfStackMemory] = "OutOfStackMemory",
  [(std::uint32_t)StateTerminationType::Replay] = "Replay",
  [7] = "",
  [8] = "",
  [9] = "",
  [(std::uint32_t)StateTerminationType::Abort] = "Abort",
  [(std::uint32_t)StateTerminationType::Assert] = "Assert",
  [(std::uint32_t)StateTerminationType::BadVectorAccess] = "BadVectorAccess",
  [(std::uint32_t)StateTerminationType::Free] = "Free",
  [(std::uint32_t)StateTerminationType::Model] = "Model",
  [(std::uint32_t)StateTerminationType::Overflow] = "Overflow",
  [(std::uint32_t)StateTerminationType::Ptr] = "Ptr",
  [(std::uint32_t)StateTerminationType::ReadOnly] = "ReadOnly",
  [(std::uint32_t)StateTerminationType::ReportError] = "ReportError",
  [19] = "",
  [20] = "",
  [(std::uint32_t)StateTerminationType::User] = "User",
  [22] = "",
  [23] = "",
  [24] = "",
  [(std::uint32_t)StateTerminationType::Execution] = "Execution",
  [(std::uint32_t)StateTerminationType::External] = "External",
  [27] = "",
  [(std::uint32_t)StateTerminationType::Merge] = "Merge",
  [(std::uint32_t)StateTerminationType::SilentExit] = "SilentExit",
  [(std::uint32_t)StateTerminationType::Solver] = "Solver",
  [31] = "",
};


///@brief Filename extensions for StateTerminationTypes
static char const * const stateTerminationTypeSuffix[32] = {
  [(std::uint32_t)StateTerminationType::RUNNING] = "",
  [(std::uint32_t)StateTerminationType::Exit] = "",
  [(std::uint32_t)StateTerminationType::Interrupted] = "early",
  [(std::uint32_t)StateTerminationType::MaxDepth] = "early",
  [(std::uint32_t)StateTerminationType::OutOfMemory] = "early",
  [(std::uint32_t)StateTerminationType::OutOfStackMemory] = "early",
  [(std::uint32_t)StateTerminationType::Replay] = "early",
  [7] = "",
  [8] = "",
  [9] = "",
  [(std::uint32_t)StateTerminationType::Abort] = "abort.err",
  [(std::uint32_t)StateTerminationType::Assert] = "assert.err",
  [(std::uint32_t)StateTerminationType::BadVectorAccess] = "bad_vector_access.err",
  [(std::uint32_t)StateTerminationType::Free] = "free.err",
  [(std::uint32_t)StateTerminationType::Model] = "model.err",
  [(std::uint32_t)StateTerminationType::Overflow] = "overflow.err",
  [(std::uint32_t)StateTerminationType::Ptr] = "ptr.err",
  [(std::uint32_t)StateTerminationType::ReadOnly] = "readonly.err",
  [(std::uint32_t)StateTerminationType::ReportError] = "report_error.err",
  [19] = "",
  [20] = "",
  [(std::uint32_t)StateTerminationType::User] = "user.err",
  [22] = "",
  [23] = "",
  [24] = "",
  [(std::uint32_t)StateTerminationType::Execution] = "exec.err",
  [(std::uint32_t)StateTerminationType::External] = "external.err",
  [27] = "",
  [(std::uint32_t)StateTerminationType::Merge] = "",
  [(std::uint32_t)StateTerminationType::SilentExit] = "",
  [(std::uint32_t)StateTerminationType::Solver] = "",
  [31] = "",
};

#endif
