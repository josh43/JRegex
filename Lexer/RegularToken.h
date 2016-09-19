//
// Created by joshua on 8/9/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGULARTOKEN_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGULARTOKEN_H

#include <string>

using namespace std;
enum TOKEN: unsigned int{
    COLL_ELEM_SINGLE = 0,
    COLL_ELEM_MULTI,    // USED For [:a-z:] aka builtin character classes
    BACKREF,            //
    DUP_COUNT,          //
    META_CHAR,          //
    L_ANCHOR,           // ^
    R_ANCHOR,           // $
    ORD_CHAR,           //
    QUOTED_CHAR,        //
    SPEC_CHAR,          // . \ [ ^  $ . *
    STAR,               //
    PLUS,               //
    DOT,
    L_CURLY,            // {
    R_CURLY,            // }
    L_PAREN,            // (
    R_PAREN,            // )
    L_BRACKET,          // [
    R_BRACKET,          // ]
    BACK_OPEN_PAREN,    // '\('
    BACK_CLOSE_PAREN,   //
    BACK_OPEN_CURLY,    // '\{'
    BACK_CLOSE_CURLY,
    QUESTION,           //
    DASH,               //
    OR,
    COMMA,              //
    EQUALS,
    QUOTE,
    UNUSED_TOKEN,
    EOF_TOKEN,

    MAX_TOKEN_VALUE

};
static const char * names[] = {
        "COLL_ELEM_SINGLE",
        "COLL_ELEM_MULTI",
        "BACKREF",
        "DUP_COUNT",
        "META_CHAR",
        "L_ANCHOR",
        "R_ANCHOR",
        "ORD_CHAR",
        "QUOTED_CHAR",
        "SPEC_CHAR",
        "STAR",
        "PLUS",
        "DOT",
        "L_CURLY",
        "R_CURLY",
        "L_PAREN",
        "R_PAREN",
        "L_BRACKET",
        "R_BRACKET",
        "BACK_OPEN_PAREN",
        "BACK_CLOSE_PAREN",
        "BACK_OPEN_CURLY",
        "BACK_CLOSE_CURLY",
        "QUESTION",
        "DASH",
        "OR",
        "COMMA",
        "EQUALS",
        "QUOTE",
        "UNUSED_TOKEN",
        "EOF_TOKEN"
};

struct RegularToken {
    TOKEN token;
    string text;
    RegularToken(TOKEN type, const string &text):token(type),text(text) { }
    RegularToken(){}
    string toString(){
        return "<" + string(names[token]) + " , " + text + ">";
    }
    ~RegularToken(){}

};


#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGULARTOKEN_H
