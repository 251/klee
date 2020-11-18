// RUN: %clang %s -emit-llvm %O0opt -g -c -o %t.bc
// RUN: rm -rf %t.klee-out
// RUN: %klee -write-ptree --output-dir=%t.klee-out %t.bc
// RUN: %klee-ptree branchinfo %t.klee-out/ptree.db | FileCheck --check-prefix=CHECK-BRANCH %s
// RUN: %klee-ptree treeinfo %t.klee-out/ptree.db | FileCheck --check-prefix=CHECK-TREE %s
// RUN: %klee-ptree dot %t.klee-out/ptree.db | FileCheck --check-prefix=CHECK-DOT %s
// RUN: not %klee-ptree dot %t.klee-out/ptree-doesnotexist.db

#include <stddef.h>

int main(void) {
  int a = 42;
  int c0, c1, c2, c3;
  klee_make_symbolic(&c0, sizeof(c0), "c0");
  klee_make_symbolic(&c1, sizeof(c1), "c1");
  klee_make_symbolic(&c2, sizeof(c2), "c2");
  klee_make_symbolic(&c3, sizeof(c3), "c3");

  if (c0) {
    a += 17;
  } else {
    a -= 4;
  }

  if (c1) {
    klee_assume(!c1);
  } else if (c2) {
    char *p = NULL;
    p[4711] = '!';
  } else if (c3) {
    klee_silent_exit(0);
  } else {
    return a;
  }

  return 0;
}

// CHECK-BRANCH: Alloc: 0
// CHECK-BRANCH: Br: 7
// CHECK-BRANCH: {{[0-9]+}}: 2
// CHECK-BRANCH: {{[0-9]+}}: 2
// CHECK-BRANCH: {{[0-9]+}}: 2
// CHECK-BRANCH: {{[0-9]+}}: 1

// CHECK-TREE: leaf nodes: 8
// CHECK-TREE: max. depth: 4
// CHECK-TREE: Exit: 2
// CHECK-TREE: Ptr: 2
// CHECK-TREE: User: 2
// CHECK-TREE: SilentExit: 2
// CHECK-TREE: 2: 2
// CHECK-TREE: 3: 2
// CHECK-TREE: 4: 4

// CHECK-DOT: strict digraph PTree {
// CHECK-DOT: N{{[0-9]+}}[tooltip="node: {{[0-9]+}}\nstate: {{[0-9]+}}\nasm: {{[0-9]+}}\nBr"];
// CHECK-DOT: N{{[0-9]+}}->{N{{[0-9]+}} N{{[0-9]+}}};
// CHECK-DOT: }