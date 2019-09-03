#!/usr/bin/python

import sys

ptrs = {}
ptrcnt = 0

print("#include \"jmalloccore.h\"")
print("#include <stdlib.h>")

print("struct op {")
print("  int is_free;")
print("  int loc;")
print("  int sz;")
print("};")

with open("allocs.txt", "r") as f:
  content = f.readlines()
  for line in content:
    op,ptr,sz = line.split(" ")
    if ptr not in ptrs:
      ptrs[ptr] = ptrcnt
      ptrcnt += 1
  print("void *ar[%d];" % (ptrcnt,))
  print("size_t sz[%d];" % (ptrcnt,))
  print("struct op ops[%d] = {" % (len(content),))
  for line in content:
    op,ptr,sz = line.split(" ")
    if op == "alloc:":
      print("  {.is_free = 0, .loc = %d, .sz = %d}," % (ptrs[ptr], int(sz)))
    else:
      print("  {.is_free = 1, .loc = %d, .sz = %d}," % (ptrs[ptr], int(sz)))
  print("};")
  print("int main(int argc, char **argv) {")
  print("  size_t i, j;")
  print("  for (j = 0; j < 100; j++) {")
  print("  for (i = 0; i < %d; i++) {" % (len(content),))
  print("    if (ops[i].is_free)")
  print("    {")
  if len(sys.argv) > 1 and sys.argv[1] == "std":
    print("      free(ar[ops[i].loc]);")
  else:
    print("      jmfree(ar[ops[i].loc], ops[i].sz);")
  print("      ar[ops[i].loc] = NULL;")
  print("      sz[ops[i].loc] = 0;")
  print("    }")
  print("    else")
  print("    {")
  if len(sys.argv) > 1 and sys.argv[1] == "std":
    print("      ar[ops[i].loc] = malloc(ops[i].sz);")
  else:
    print("      ar[ops[i].loc] = jmalloc(ops[i].sz);")
  print("      sz[ops[i].loc] = ops[i].sz;")
  print("    }")
  print("  }")
  print("  for (i = 0; i < %d; i++) {" % (ptrcnt,))
  if len(sys.argv) > 1 and sys.argv[1] == "std":
    print("    if (ar[i]) {free(ar[i]); ar[i] = NULL;}")
  else:
    print("    if (ar[i]) {jmfree(ar[i], sz[i]); ar[i] = NULL;}")
  print("  }")
  print("  }")
  print("}")
