//
// Created by joshua on 8/10/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGULARTREENODES_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGULARTREENODES_H


#include <vector>
#include <stack>
#include <deque>
#include "../Lexer/RegularToken.h"
#include <stdexcept>
static const unsigned int OR_NODE =         1 << 0;
static const unsigned int STAR_NODE =       1 << 1;
static const unsigned int PLUS_NODE =       1 << 2;
static const unsigned int QUESTION_NODE =   1 << 3;
static const unsigned int DATA_NODE =       1 << 4;
static const unsigned int SPEC_DATA_NODE =  1 << 5;
static const unsigned int BRACKET_NODE =    1 << 6;
static const unsigned int NEGATED_BRACKET=  1 << 7;
static const unsigned int AND_NODE=         1 << 8;
static const unsigned int SENTINEL=         1 << 9;
static const vector<string> print_node_names = {
        "OR_NODE",
                                   "STAR_NODE",
                                   "PLUS_NODE",
                                   "QUESTION_NODE",
                                   "DATA_NODE",
                                   "SPEC_DATA_NODE",
                                   "BRACKET_NODE",
                                   "NEGATED_BRACKET",
                                   "AND_NODE",
                                    "SENTINEL"

};
static const vector<unsigned int> print_node_values = {OR_NODE,
                                                       STAR_NODE,
                                                       PLUS_NODE,
                                                       QUESTION_NODE,
                                                       DATA_NODE,
                                                       SPEC_DATA_NODE,
                                                       BRACKET_NODE,
                                                       NEGATED_BRACKET,
                                                       AND_NODE,
                                                       SENTINEL
                                            };
using namespace std;
struct RegularNode {
    RegularNode * left;
    RegularNode * right;

    unsigned int nodeType;
    string regularData;
    RegularNode():left(nullptr),right(nullptr),nodeType(DATA_NODE),regularData(""){

    }
    RegularNode(unsigned int type):RegularNode(){
        nodeType = type;
    }
    ~RegularNode(){



    }


};

class RegularDeque {
public:
    bool lAnchor, rAnchor;
    std::deque<RegularNode * > regularDeque;
    RegularDeque(){ lAnchor = false, rAnchor = false;}
    void push(RegularNode * node){
        regularDeque.push_back(node);
    }

    unsigned int size(){
        return regularDeque.size();
    }
    RegularNode * pop(){RegularNode * toReturn = regularDeque.back(); regularDeque.pop_back(); return toReturn;}
    bool isEmpty(){return regularDeque.empty(); }


    void combineLastTwoExpressions(unsigned int orExp = 0){
        if(regularDeque.size() < 2){throw std::invalid_argument("Stack underflow");}
        RegularNode * second = this->pop();
        RegularNode * first =this->pop();
        RegularNode * toPush = new RegularNode();
        toPush->left = first; toPush->right = second;
        if(orExp){
            toPush->nodeType = OR_NODE;
        }else{
            toPush->nodeType = AND_NODE;
         }
        regularDeque.push_back(toPush);

    }
    void combineFrontTwo(){
        if(regularDeque.size() < 2){throw std::invalid_argument("Stack underflow");}
        RegularNode * first= regularDeque.front(); regularDeque.pop_front();
        RegularNode * second  =regularDeque.front(); regularDeque.pop_front();
        RegularNode * toPush = new RegularNode();
        toPush->left = first; toPush->right = second;
       // if(orExp){
       //     toPush->nodeType = OR_NODE;
       // }else{
            toPush->nodeType = AND_NODE;
        //}
        regularDeque.push_front(toPush);
    }

    void duplSymbolLast(RegularNode *& regular,RegularToken duplSymbol,bool newNode = false){
        unsigned int duplValue = duplSymbol.token == OR ? OR_NODE : duplSymbol.token == PLUS? PLUS_NODE  : duplSymbol.token == STAR ? STAR_NODE : QUESTION_NODE;
        regular->nodeType |= duplValue;
    }
};





#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGULARTREENODES_H

