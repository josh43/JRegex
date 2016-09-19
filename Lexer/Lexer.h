//
// Created by joshua on 8/9/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_LEXER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_LEXER_H

#include "RegularToken.h"
#include <limits.h>
#include <stdexcept>


using namespace std;
#define UNRECOGNIZED_TOKEN() \
 printf("\"Unrecognized token : %c \n @ %s",c,__PRETTY_FUNCTION__);\
    throw std::invalid_argument("")



class Lexer{
public:
    string input;
    unsigned char c; // current character
    unsigned int index;
    static const unsigned int EOF_TOKEN_VALUE = (1 << 8) - 1;

    Lexer(string input):input(input){
        index = 0;
        c = input[index];

    }

    virtual ~Lexer(){

    }
    virtual RegularToken nextToken() = 0;
    void consume(){
        index++;
        if(index >= input.length()){
            c = EOF_TOKEN_VALUE;
        }else{
            c = input[index];
            if(c == EOF_TOKEN_VALUE){
                throw std::invalid_argument("Unable to parse regexps that contain EOF_TOKEN_VALUE = 255\n");
            }
        }
    }
    void match(char x){
        if(c == x){
            consume();
        }else{
            UNRECOGNIZED_TOKEN();
        };
    }

};


#endif //LANGUAGEIMPLEMENTATIONPATTERNS_LEXER_H
