//
// Created by josh on 9/14/16.
//

#ifndef JLEXX_REGULARERRORMESSAGE_H
#define JLEXX_REGULARERRORMESSAGE_H


#include <cstring>
#include <cstdio>
#include "../Lexer/Lexer.h"
/* Obviously not worrying about mem leak because these are CRASH Messages
 *
 */
static char * badRegexp(Lexer * lex){
    char * error = new char[200];
    memset(error,0,sizeof(char) * 200);
    sprintf(error,"BAD RE %s\n",lex->input.c_str());
    return error;
}

static char * badRegexp(Lexer * lex,const char * c){
    char * error = new char[300];
    memset(error,0,sizeof(char) * 200);
    sprintf(error,"BAD RE %s",lex->input.c_str());
    sprintf(error+lex->input.size() + 8,"%s",c);
    return error;
}
#endif //JLEXX_REGULARERRORMESSAGE_H
