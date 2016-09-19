#!/usr/bin/env bash
grep -o "#include" Test.h                               > grout/t1.out
grep -o "bool" Test.h                                   > grout/t2.out
grep -o "//[^\n]*[\n]" Test.h                           > grout/t3.out
grep -o "[a-zA-Z][a-zA-Z0-9]*" Test.h                   > grout/t4.out
grep -o  "[a-zA-Z][a-zA-Z0-9]*[(][^)]*[)]" Test.h       > grout/t5.out


