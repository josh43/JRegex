//
// Created by joshua on 8/17/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGEXEXAMPLES_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGEXEXAMPLES_H

#include "../Regex.h"

// floating point
// ^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$
// ^ == $ [ \n\t]
using namespace JRegex;
void regexExamplesOne(){
    Regex * regex    = Regex::createRegex("lol");
   Regex * regex1   = Regex::createRegex("^abcd.*$");
   Regex * regex2   = Regex::createRegex("^[a-z][a-z]*");
   Regex * regex3   = Regex::createRegex("mop$");

   // broken vvvvv
  regex->verbose().matchLongest().match("lololololol").output();
  //vvv will only match one time as expected
  regex1->verbose().matchLongest().match("abcdabcd aaabcd abcd abcd").output();
  regex2->verbose().matchLongest().match("aaadad aioz9dvd").output();
  regex3->verbose().matchLongest().match("mmmmop mopp mop\n mop\t").output();

    delete regex;
    delete regex1;
    delete regex2;
    delete regex3;
}
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGEXEXAMPLES_H
