//===-- CoreStats.cpp -----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "CoreStats.h"

using namespace klee;

Statistic stats::allocations("Allocations", "Alloc");
Statistic stats::coveredInstructions("CoveredInstructions", "Icov");
Statistic stats::falseBranches("FalseBranches", "Bf");
Statistic stats::forkTime("ForkTime", "Ftime");
Statistic stats::forks("Forks", "Forks");
Statistic stats::instructionRealTime("InstructionRealTimes", "Ireal");
Statistic stats::instructionTime("InstructionTimes", "Itime");
Statistic stats::instructions("Instructions", "I");
Statistic stats::minDistToReturn("MinDistToReturn", "Rdist");
Statistic stats::minDistToUncovered("MinDistToUncovered", "UCdist");
Statistic stats::reachableUncovered("ReachableUncovered", "IuncovReach");
Statistic stats::resolveTime("ResolveTime", "Rtime");
Statistic stats::solverTime("SolverTime", "Stime");
Statistic stats::states("States", "States");
Statistic stats::termExit("TExit", "TExit");
Statistic stats::termEarly("TEarly", "TEarly");
Statistic stats::termProgErr("TProgramError", "TError");
Statistic stats::termUserErr("TUserError", "TUserErr");
Statistic stats::termExecErr("TExecutionError", "TExecErr");
Statistic stats::trueBranches("TrueBranches", "Bt");
Statistic stats::uncoveredInstructions("UncoveredInstructions", "Iuncov");
