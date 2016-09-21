#!/usr/bin/env bash
mkdir out
mkdir diff

regexes=("#include"
        "bool"
        "//[^\n]*[\n]"
        "[a-zA-Z][a-zA-Z0-9]*"
        "[a-zA-Z][a-zA-Z0-9]*[(][^)]*[)]"
        "a*"
        "A+"
        "[a-z]+\[[0-9]+\][A-Z]+"
        "[a-z]+ [a-zA-Z]+[ ]?{[^}]*}"
        "if[ ]*[(][^)]*[)][ ]*{[^}]*}"
        )

passed=0;
failed=0;
total=${#regexes[@]};
for (( x=0; x <${total}; x++ ));
do
    echo "Searching regexp ${regexes[x]}"

  egrep -o "${regexes[x]}"  Test.h > out/t${x}.out
  ./main "${regexes[x]}" <    Test.h > out/m${x}.out
  diff out/t${x}.out out/m${x}.out > diff/Test${x}.out

  # -s is size of file
  if [ -s  diff/Test${x}.out ]
  then
    echo "Failed test ${x}";
    (( failed++ ));
  else
    rm out/t${x}.out
    rm out/m${x}.out
    rm diff/Test${x}.out
    (( passed++ ));
  fi


done

percent=$(( 100 / total ));
passPercent=$(( percent * passed ));
echo "Passed ${passed} tests"
echo "Pass Percent ${passPercent}"
echo "Failed ${failed} tests"

