//
// Created by joshua on 8/9/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGULARLEXER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGULARLEXER_H


#include "Lexer.h"

class RegularLexer : public Lexer {
public:

    RegularLexer(string input): Lexer(input) {
            bracketOpenCount = 0;
    }
    // dont pass in [-asd]
    //               ^


    virtual RegularToken nextToken() override{
        while(c != EOF_TOKEN_VALUE){
            switch(c){
               // case '\t': case '\n': case '\r': throw std::invalid_argument("Regexps dont accept tabs newlines or carriage returns \n");
                case '\0': return RegularToken(EOF_TOKEN,"<EOF>");
                case '\\': return escapeToken();
                case '[' :consume(); return RegularToken(L_BRACKET,"[");
                case ']' :consume(); return RegularToken(R_BRACKET,"]");
                case '*' :consume(); return RegularToken(STAR,"*");
                case '+' :consume(); return RegularToken(PLUS,"+");
                case '?' :consume(); return RegularToken(QUESTION,"?");
                case '^' :consume(); return RegularToken(L_ANCHOR,"^");
                case '$' :consume(); return RegularToken(R_ANCHOR,"$");
                case '|' :consume(); return RegularToken(OR,"|");
                case '(' :consume(); return RegularToken(L_PAREN,"(");
                case ')' :consume(); return RegularToken(R_PAREN,")");
               // case '\'':consume(); return RegularToken(QUOTE,"'");
                case '-' :consume(); return RegularToken(DASH,"-");
                case '.' :consume(); return RegularToken(DOT,".");
                default  :string str; str.push_back(c); consume();
                                     return RegularToken(ORD_CHAR,str); // check this

            }
        }
        return RegularToken(EOF_TOKEN,"<EOF>");
    }



private:
    // no longer using but keeping around just in case vvvvv
    RegularToken escapeToken(){
        string estr; estr.push_back(c);
        consume();
        if(this->index >= this->input.size()){
            throw std::invalid_argument("Not legal to have --> \\ <-- at end of regexp must do \\\\ ");
        }
        estr.push_back(c); consume();
        return RegularToken(SPEC_CHAR,estr);
    }
    void pushBr(){
        bracketOpenCount++;
    }
    void popBr(){
        bracketOpenCount--;
    }
    bool insideBr(){
        return bracketOpenCount;
    }

    int bracketOpenCount;
};


#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGULARLEXER_H
