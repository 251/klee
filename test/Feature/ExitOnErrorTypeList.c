// RUN: %clang %s -g -emit-llvm %O0opt -c -o %t.bc
// RUN: rm -rf %t_dfs.klee-out
// RUN: rm -rf %t_bfs.klee-out
// RUN: %klee -search=dfs --output-dir=%t_dfs.klee-out -exit-on-error-type=Assert,Abort %t.bc 2>&1 | FileCheck --check-prefix=CHECK-DFS %s
// RUN: %klee -search=bfs --output-dir=%t_bfs.klee-out -exit-on-error-type=Assert,Abort %t.bc 2>&1 | FileCheck --check-prefix=CHECK-BFS %s

#include "klee/klee.h"

#include <assert.h>
#include <stdlib.h>

int main() {
  int cond = klee_int("condition");

  if (cond < 42) {
    assert(0);
  } else {
    abort();
  }

  return 0;
}

// CHECK-DFS: KLEE: ERROR: {{.*}}test/Feature/ExitOnErrorTypeList.c:{{18: abort failure|16: ASSERTION FAIL: .*}}
// CHECK-BFS: KLEE: ERROR: {{.*}}test/Feature/ExitOnErrorTypeList.c:{{16: ASSERTION FAIL: .*|18: abort failure}}