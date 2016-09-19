#!/usr/bin/env bash

outFiles=(t1.out t2.out t3.out t4.out t5.out)
#"a*")); // include.out
#er("A+")); // include.out
#Lexer("[A-Z]+")); // include.out
#Lexer("[a-z]+ [a-zA-Z]+[ ]?{[^}]*}"));
#larLexer("if[ ]*[(][^)]*[)][ ]*{[^}]*}"





patterns=("a*" "A+" "[a-z]+\[[0-9]+\]"[A-Z]+" "[a-z]+ [a-zA-Z]+[ ]?{[^}]*}" "if[ ]*[(][^)]*[)][ ]*{[^}]*}")
groutDir=grout
outDir=out
#{name[i]} --> {outFiles[0]} @ means all
#for item in ${outFiles[@]}
#do
#    grep -o
#done

for((x=0;x<5;x++))
do
    echo "searching pattern" ${patterns[$x]} sending to $groutDir/${outFiles[$x]}
    egrep -o ${patterns[$x]} Test.h > grout/${outFiles[$x]}
done
#grep -o "#include" Test.h                               > grout/t1.out
#grep -o "bool" Test.h                                   > grout/t2.out
#grep -o "//[^\n]*[\n]" Test.h                           > grout/t3.out
#grep -o "[a-zA-Z][a-zA-Z0-9]*" Test.h                   > grout/t4.out
#grep -o  "[a-zA-Z][a-zA-Z0-9]*[(][^)]*[)]" Test.h       > grout/t5.out # none greedy


