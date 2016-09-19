//
// Created by joshua on 8/15/16.
//
#include "Utility.h"

using namespace std;
void printError(string errorMessage,string str,char expected,int hint){

    int index = str.find(expected,hint);
    printf("%s\n",errorMessage.c_str());
    printf("%s\n",str.c_str());
    printf("%*c\n",index + 1,'^');

}