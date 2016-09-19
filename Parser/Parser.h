//
// Created by joshua on 8/10/165.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_PARSER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_PARSER_H

#endif //LANGUAGEIMPLEMENTATIONPATTERNS_PARSER_H
#include <vector>
#include <unordered_map>
#include <assert.h>
#include "../Lexer/Lexer.h"
#include "../Lexer/RegularToken.h"
#include <stdexcept>

using namespace std;
class Parser {
public:
    Lexer  * lexer;
    unsigned int curr;
    const unsigned int FAILED =  std::numeric_limits<unsigned int>::max() -1;
    vector<RegularToken> lookAhead;
    vector<unsigned int> markers;
    unordered_map<unsigned int,unsigned int> map;
public:
    Parser(Lexer * l):lexer(l) {
        //,"CANNOT HAVE IT LONGER THAN  1<< 32 -2"
        assert(l->input.size() < FAILED);
        curr = 0;

    }

    virtual ~Parser() {
        if(lexer){
            delete lexer;
        }
    }

    void memoize(unsigned int start, bool failed){
        map.insert(pair<unsigned int, unsigned int>(start,failed ? FAILED : curr));
    }
    bool parsedRuleAtLocation(unsigned int index){
        if(map.at(index)){
            if(map[index] != FAILED){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    void match(const uint val) {
        if(getLookaheadTypeAt(1) == val){
            this->consume();
        }else{
            printf(" bad token : %s expected : %s \n %s\n",this->getLookaheadAt(1).toString().c_str(),getLookaheadAt(1).toString().c_str(),__PRETTY_FUNCTION__);
            throw std::invalid_argument("");
        }
    }
    void consume() {

        curr++;
        if(curr == lookAhead.size() && !speculating()){
            curr = 0;
            lookAhead.clear();
        }
        syncLookahead(1);
    }
    bool speculating(){return this->markers.size();}

    RegularToken getLookaheadAt(unsigned int i) {
        syncLookahead(i);

        return lookAhead[(curr + i - 1)];
    }
    unsigned int getLookaheadTypeAt(unsigned int i) {
        return getLookaheadAt(i).token;
    }

    void syncLookahead(unsigned int i){
        unsigned size= 0;
        if((size = curr + i - lookAhead.size()) > 0){
            for(uint i = 0; i < size; i ++){
                lookAhead.push_back(lexer->nextToken());
            }
        }
    }
    int mark(){
        markers.push_back(curr);
        return curr;
    }
    void release(){
        int marker = markers.back();
        markers.pop_back();
        seek(marker);
    }
    void seek(int index){
        curr = index;
    }




};