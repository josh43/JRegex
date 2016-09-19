#include <iostream>
#include "Regex/Regex.h"

int main() {
    using namespace JRegex;
    // when using or expressions you ened to surround them with parenthesis
    Regex * reg2 = Regex::createRegex("([a-z]+|[0-9]+)"); // this will take either a string of letters or a string of numbers
    Regex * reg3 = Regex::createRegex("([a-z0-9]+)"); // this will take a-z or 0-9 and one or more
    Regex * reg4 = Regex::createRegex("[\n\t ][a-zA-Z]+");
    reg2->match("hello nd329 o3k",false).output();
    reg3->match("hello nd329 o3k",false).output();
    printf("Begin reg4\n");
    // note that i pad the beginning and ending of matches with a \n
    reg4->match("hello nd329 o3k",false).output();

    return 0;
}