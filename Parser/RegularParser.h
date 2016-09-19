//
// Created by joshua on 8/10/16.
// TODO make grammar into ( RE_Expression ) | RE_Expression

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGULARPARSER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGULARPARSER_H


#include <stack>
#include "Parser.h"
#include "../Utility.h"
#include "RegularTreeNodes.h"
#include <stack>
#include <string.h>
#include "RegularErrorMessage.h"

class RegularParser : public Parser {
    /*
 basic_reg_exp  :          RE_expression
               | L_ANCHOR
               |                        R_ANCHOR
               | L_ANCHOR               R_ANCHOR
               | L_ANCHOR RE_expression
               |          RE_expression R_ANCHOR
               | L_ANCHOR RE_expression R_ANCHOR
               ;

RE_expression    : ( RE_expression ) opDuplSymbol
                 : ( RE_expression  | RE_expression )  opDuplSymbol
                 : expr RE_expression
                 : expr


RE_Right_expr  |
               ; simple_RE RE_expression
               ;
simple_RE      : nondupl_RE
               | nondupl_RE RE_dupl_symbol
               ;
nondupl_RE     : one_char_or_coll_elem_RE
               | BACKREF
               ;
one_char_or_coll_elem_RE : ORD_CHAR
               | QUOTED_CHAR
               | '.'
               | bracket_expression
               ;
RE_dupl_symbol : '*'
               | '+'
               | '?'
               TODO THIS ---> Back_open_brace DUP_COUNT               Back_close_brace
               TODO THIS ---> Back_open_brace DUP_COUNT ','           Back_close_brace
               TODO THIS ---> Back_open_brace DUP_COUNT ',' DUP_COUNT Back_close_brace
 */
public:

    //
    string regularExpression;
    RegularDeque deque;

    RegularParser(Lexer *l) : Parser(l),regularExpression("") { }
    void parse() {
        basicRegexp();
        JAssert(this->deque.size() > 1,badRegexp(this->lexer,"Error parsing regular expression stack size should be 1"))
    }


private:

    void basicRegexp() {
        JAssert(this->getLookaheadTypeAt(1) == EOF_TOKEN,badRegexp(this->lexer,"Reached EOF token early"))



            if (this->getLookaheadTypeAt(1) == L_ANCHOR) {
                match(L_ANCHOR);
                deque.lAnchor  = true;
                prettyPrint("L_ANCHOR");
                JAssert(this->getLookaheadTypeAt(1) == R_ANCHOR,badRegexp(this->lexer));

                mark();
                if (RE_expression()) {
                    while(RE_expression()){ deque.combineLastTwoExpressions();}
                    if (this->getLookaheadTypeAt(1) == R_ANCHOR) {
                        match(R_ANCHOR);
                        deque.rAnchor = true;
                        return;
                        //L_ANCHOR RE R_ANCHOR
                    } else {
                        // LANCHOR  RE
                    }
                } else {
                    release();
                    JAssert(true,badRegexp(this->lexer,"Cannot have a plain ^"))

                }

        } else if (this->getLookaheadTypeAt(1) == R_ANCHOR) {
            match(R_ANCHOR);
                deque.rAnchor = true;
            prettyPrint("R_ANCHOR");
            return;
        }else {
                mark();
                if (RE_expression()) {
                    while (RE_expression()) { deque.combineLastTwoExpressions(); }
                    prettyPrint("RE_expression");
                    if (this->getLookaheadTypeAt(1) == R_ANCHOR) {
                        match(R_ANCHOR);
                        deque.rAnchor = true;
                    } else {
                    }

                } else {
                    release();
                    JAssert(true,badRegexp(this->lexer));

                }
            }
    }

    bool RE_expression() {

        if (this->getLookaheadTypeAt(1) == L_PAREN) {
            // try and do expression
            mark();
            match(L_PAREN);
            if (RE_expression()) { // this should succeed for case 12
                while(this->getLookaheadTypeAt(1) == OR) {
                    mark();
                    match(OR);
                    // try and do second RE
                    if (RE_expression()) {
                        // just fall off
                        deque.combineLastTwoExpressions(OR);
                    } else {
                        JAssert(true,badRegexp(this->lexer,"Expecting another expression after |"));

                    }
                }

                if (this->getLookaheadTypeAt(1) == R_PAREN) {
                    regularExpression.push_back(')');
                    match(R_PAREN);
                    JAssert(deque.isEmpty(),badRegexp(this->lexer,"Stack shouldnt be empty!"));

                    RegularNode * top = deque.pop();
                    opDuplSymbol(top,2);
                    deque.push(top);
                   //stack.push(top); dont push back in this case opDupl will do for you!
                    return true;
                } else {
                    return false;
                }
            } else {
                release();
                return false; // failed
            }
        }
        mark();
        if (simple_RE()) {
            mark();

            if (RE_expression()) {
                deque.combineLastTwoExpressions();
            } else {
                release();
            }
            return true;
        } else {
            release();
            return false;
        }
    }

    /* simple_RE        : oneOrMoreSymbols
     *                  : BRE
     *                  : BRE duplSymbol
     * oneOrMoreSymbols : ORD_CHAR opWildCardSymbols oneOrMoreSymbols
     *                  : ORD_CHAR opWildCardSymbols
     *
     * opWildCardSymbols: '.' '*'
     *                  : '.' '+'
     *                  : '.'
     *                  : '+'
     *                  : '*'
     *                  : '?'
     *                  :
     *
     */
    bool simple_RE() {
        mark();

        if (oneOrMoreChars()) {
            return true;
        }
        release();
        mark();
        if (BRE()) {
            return true;
        }
        release();
        return false;

    }

    bool oneOrMoreChars() {
        RegularNode *  gen = nullptr;
       bool matched = false;
        bool shouldCombine = false;
        while(true) {
            switch(this->getLookaheadTypeAt(1)) {
                case SPEC_CHAR:
                case ORD_CHAR: case DOT:
                    gen = new RegularNode();
                    gen->nodeType = DATA_NODE;
                    if(this->getLookaheadAt(1).text.front() == '\\'){
                        gen->regularData += this->getLookaheadAt(1).text[1];

                    }else {
                        gen->regularData += this->getLookaheadAt(1).text;
                    }
                    match(this->getLookaheadTypeAt(1));
                    opDuplSymbol(gen);
                    deque.push(gen);
                    matched = true;
                    if(shouldCombine){
                        deque.combineLastTwoExpressions();
                    }
                    shouldCombine = true;
                    continue;
                default:
                    return matched;

            }
        }
    }

    void opDuplSymbol(RegularNode * regular,unsigned int expression = 0){
        // 0 -- basic symbol on letter or a SPEC Char like  2
        // 1->inf -- expression  2
        switch (this->getLookaheadTypeAt(1)) {
            case QUESTION:
            case STAR:
            case PLUS:
               // if(!expression) {
               //     stack.duplSymbolLast(regular, this->getLookaheadAt(1));
               // }else{
                   //RegularNode * dupNode = new RegularNode();dupNode->left = regular;
                   //dupNode->nodeType = 0;
                    deque.duplSymbolLast(regular, this->getLookaheadAt(1));
                  //  stack.push(regular);
                //}
                match(this->getLookaheadTypeAt(1));

                return;
            default:
                return;
        }
    }

    bool BRE() {
        mark();


        if (this->getLookaheadTypeAt(1) == L_BRACKET) {
            RegularNode * regular = new RegularNode();
            regular->nodeType = BRACKET_NODE;
            match(L_BRACKET);
            if (this->getLookaheadTypeAt(1) == L_ANCHOR) {
                regular->nodeType |= NEGATED_BRACKET;
                match(L_ANCHOR); // negation
            }
            // match everything
            int charactersBeforeDash = 0;
            while (this->getLookaheadTypeAt(1) != R_BRACKET) {


                JAssert(this->getLookaheadTypeAt(1) == EOF_TOKEN,badRegexp(this->lexer));
                //assert(this->getLookaheadTypeAt(1) != EOF_TOKEN);
                if (this->getLookaheadTypeAt(1) == DASH) {
                    JAssert(this->getLookaheadTypeAt(1) == EOF_TOKEN,badRegexp(this->lexer,"There must be characters before the dash in a breacket expression!"));

                    charactersBeforeDash = 0;

                }

                regular->regularData += this->getLookaheadAt(1).text;
                charactersBeforeDash++;
                match(this->getLookaheadTypeAt(1));

            }
            match(R_BRACKET);
            opDuplSymbol(regular,true);
            deque.push(regular);
            return true;
        }else{
            return false;
        }
    }




};

#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGULARPARSER_H

