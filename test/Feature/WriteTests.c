// RUN: %clang %s -emit-llvm %O0opt -c -o %t.bc
// # default: all
// RUN: rm -rf %t_default.klee-out
// RUN: %klee -max-depth=1 --output-dir=%t_default.klee-out %t.bc 2>&1 | FileCheck --check-prefix=CHECK-DEFAULT %s
// # none
// RUN: rm -rf %t_none.klee-out
// RUN: %klee -max-depth=1 -write-tests=none --output-dir=%t_none.klee-out %t.bc 2>&1 | FileCheck --check-prefix=CHECK-NONE %s
// # none-combined
// RUN: rm -rf %t_nonecombined.klee-out
// RUN: not %klee -max-depth=1 -write-tests=none,error --output-dir=%t_nonecombined.klee-out %t.bc 2>&1 | FileCheck --check-prefix=CHECK-NONECOMBINED %s
// # selection (early, error)
// RUN: rm -rf %t_selection.klee-out
// RUN: %klee -max-depth=1 -write-tests=early,error --output-dir=%t_selection.klee-out %t.bc 2>&1 | FileCheck --check-prefix=CHECK-SELECTION %s

int main(void) {
  char c, *p;
  int cond1 = klee_int("condition1");
  int cond2 = klee_int("condition2");

  switch (cond1) {
    // 1x program error
  case 1:
    p = (char *)0xdeadbeef;
    c = p[4711];
    // 2x early termination (max-depth)
  case 2: {
    if (cond2 < 42)
      return 2;
  }
    // 1x user error
  case 3:
    klee_assume(0);
    // 1x normal exit
  default:
    return 1;
  }
}

// CHECK-DEFAULT: KLEE: done: generated tests = 5
// CHECK-NONE: KLEE: done: generated tests = 0
// CHECK-NONECOMBINED: KLEE: ERROR: --write-test=none combined with other test category
// CHECK-SELECTION: KLEE: done: generated tests = 3
