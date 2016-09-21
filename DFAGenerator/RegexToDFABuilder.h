//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_REGEXTODFABUILDER_H
#define JLEXX_REGEXTODFABUILDER_H

#include <iosfwd>
#include <vector>
#include "../NFAGenerator/NFAGenerator.h"
#include "../NFAGenerator/EZGraph.h"
#include "DFABuilderHelper.h"
#include "../Lexer/RegularLexer.h"
#include "../Parser/RegularParser.h"

/* TODO make the match algorithm more efficient
   TODO after creating DFA states allow it to be loaded  from a file
 */

/*  This class takes 1 or more regeular expressions and constructs a thompson e-nfa from all of them
 *  and than constructs an equivalent dfa although it is not a minumum dfa
 *  it products a vector<JRegex::State>
 *
 *
 *
 *
 *   This class allows dfa compression
 *   the one I use to parse this input, I had a compression ratio of 20, so I used about 5% as much mem as normal
 *   The downfall is that when moving on states
 *
 *   normall its
 *   vector<vector<uint> > jumpTable
 *   jumpTable[currentSate][inputCharacter] == some state
 *
 *   but with compression its
 *   so worse case it will take 8 searches
 *   u_char compressedIndex = compressState.binSearch(inputCharacter);
 *   return jumpTable[currentState][compressedIndex];
 *
 *   But it could still potentially be faster
 *
 *
 *
 *
 */
class RegexToDFABuilder{
protected:
    typedef std::pair<JRegex::Vertex *, JRegex::Vertex *> VPair;
    typedef std::pair<JRegex::Vertex *, unsigned int> Priority;


    uint                                              streamLastAccepting;
    uint                                              streamLastState;
    std::string                                       streamString;
    bool                                              hasBeenCreated;
    unsigned int                                      maxState;
    std::vector<std::string>                          regexList;
    vector<JLexx::State >                             uncompressedStateList;
    vector<JLexx::MinimizedState >                    compressedStateList;

    // This will be the head of the who entire NFA
    VPair                                             start;
    std::unordered_map<JRegex::Vertex *,unsigned int> acceptList;


public:
    RegexToDFABuilder(){

        hasBeenCreated = false;
        streamLastAccepting = JLexx::STATE_SENTINEL;
        streamLastState = 0;
        streamString.reserve(1024);


    }

    // when adding to the regexList the first added regex will have the highest precedence
    // and the second will have the next highest precedence
    RegexToDFABuilder & add(const std::string & str){
        regexList.push_back(str);
        return *this;
    }
    void addToList(const std::string & str){
        regexList.push_back(str);

    }
    RegexToDFABuilder & minimizeStates(bool verbose = false){
        if(!hasBeenCreated){
            throw std::invalid_argument("Cannot minimize states when you have not constructued them yet\n!");
        }
        uint totalSpaceMinimized = 0;
        compressedStateList.reserve(uncompressedStateList.size());
        for(auto x : this->uncompressedStateList){
            JLexx::MinimizedState minimizedState(x);
            if(verbose) {
                minimizedState.printSelf();
            }
            totalSpaceMinimized+=minimizedState.spaceUsed;
            //x.printStateInfoToFile(stdout);
            for(uint i = 0; i < 256; i++) {
                uint debug = 0;
                if(minimizedState[i] != x.jump[i]){
                    printf("This occured at state %d on State.jump[%u] = %u and Minimized.jump[%u] = %u\n",x.stateNumber,i,x.jump[i],minimizedState.binSearch(i,debug),minimizedState[i]);
                    printf("This occured at state %d on State.jump[%u] = %u and Minimized.jump[%u] = %u\n",x.stateNumber,i,x.jump[i],i,minimizedState[i]);
                    throw std::invalid_argument("Error when minimizing states, the jump results did not match");
                    printf("THIS IS PROBABLY AN ERROR!!");
                }else{
                    if(verbose) {
                        printf("It took bin search %u tries\n", debug);
                        minimizedState.printSavingsInfo();
                    }
                }
            }
            compressedStateList.push_back(std::move(minimizedState));
        }


        if(verbose) {
            ulong uncompressedSize = uncompressedStateList.size() * sizeof(JLexx::State);
            printf("\n\n COMPRESSION RATIO -> Total space saved %f or %u/%lu \n\n",
                   (float) totalSpaceMinimized / (float) uncompressedSize, totalSpaceMinimized, uncompressedSize);
        }

        // erase it
        uncompressedStateList.resize(0);
        return *this;
    }
    // Delete the start Vertex
    virtual ~RegexToDFABuilder(){
        if(start.first) {
            start.first->removeAll();
        }

    }
    // must be called in order to build
    RegexToDFABuilder & create(bool verbose = true){
        if(hasBeenCreated){
            throw std::invalid_argument("This Converter has already been created");
        }else{
            hasBeenCreated = true;
            createStates(verbose);
        }
        return *this;
    }




    // NOTE : I am not using virtual functions because they are slow so match and matchToken are basically just duplicated
    virtual bool match(const string & s, bool verbose = true, bool shortestMatch = false){
        if(shortestMatch){throw std::invalid_argument("I need to implement that feat");}
        if(compressedStateList.size()){
            return compressedMatch(s,verbose,shortestMatch);
        }
        unsigned int curr =      0;
        unsigned int start =     0;
        unsigned int lastState = 0;
        bool toReturn = false;

        unsigned int lastAcceptingState= JLexx::STATE_SENTINEL;
       //if((stateList[lastState]['\n']) != JLexx::STATE_SENTINEL){
       //    lastState = stateList[lastState]['\n'];
       //    if(stateList[lastState].isAccepting()){
       //        lastAcceptingState = lastState;
       //    }
       //}
        // I don't check after advancing if start < s.length but it is not neccessary
        // because every state['\0'] == STATE_SENTINEL
        // so it will eventually reach that :)
        while(start  < s.length()){
            while((uncompressedStateList[lastState][s[curr]]) != JLexx::STATE_SENTINEL){
                lastState = uncompressedStateList[lastState][s[curr++]];
                if(uncompressedStateList[lastState].isAccepting()){
                    lastAcceptingState = lastState;
                }
            }
            if(lastAcceptingState != JLexx::STATE_SENTINEL){
                if(s[curr-1] == '\n'){
                    printf("%s", s.substr(start, curr - start).c_str());

                }else{
                    printf("%s\n", s.substr(start, curr - start).c_str());

                }
                start+=(curr-start) - 1;
            }

            lastAcceptingState = JLexx::STATE_SENTINEL;
            lastState = 0;
            start++;
            curr = start;
        }

        return toReturn;
    }
    // Same function as match except I use the compressed Version
private:

    bool compressedMatch(const string &s, bool verbose, bool match) {
        unsigned int curr =      0;
        unsigned int start =     0;
        unsigned int lastState = 0;
        bool toReturn = false;

        unsigned int lastAcceptingState= JLexx::STATE_SENTINEL;
        //if((stateList[lastState]['\n']) != JLexx::STATE_SENTINEL){
        //    lastState = stateList[lastState]['\n'];
        //    if(stateList[lastState].isAccepting()){
        //        lastAcceptingState = lastState;
        //    }
        //}
        // I don't check after advancing if start < s.length but it is not neccessary
        // because every state['\0'] == STATE_SENTINEL
        // so it will eventually reach that :)
        while(start  < s.length()){
            while((compressedStateList[lastState][s[curr]]) != JLexx::STATE_SENTINEL){
                lastState = compressedStateList[lastState][s[curr++]];
                if(compressedStateList[lastState].isAccepting()){
                    lastAcceptingState = lastState;
                }
            }
            if(lastAcceptingState != JLexx::STATE_SENTINEL){

                if(s[curr-1] == '\n'){
                    printf("%s", s.substr(start, curr - start).c_str());
                }else{
                    printf("%s\n", s.substr(start, curr - start).c_str());
                }


                start+=(curr-start) - 1;
            }

            lastAcceptingState = JLexx::STATE_SENTINEL;
            lastState = 0;
            start++;
            curr = start;
        }

        return toReturn;
    }

    // creates the entire thompson e-nfa
    void createStates(bool verbose){
        using namespace JRegex;
        using namespace JLexx;
        maxState = 0;
        if(!regexList.size()){throw std::invalid_argument("When creating states you need at least one regex");}
         start = {nullptr,nullptr};
        for( int i = regexList.size()-1; i >=0; i--) {
            // create and parse the regular expression
            RegularParser p(new RegularLexer(regexList[i]));
            p.parse();
            // create the e-nfa graph from it
            RegularGraph graph(p.deque.regularDeque.front(),p.deque.lAnchor,p.deque.rAnchor);
            graph.dontDeallocate();
            VPair curr;

            if(start.first == nullptr) {
                start = graph.getGraph();
                start.second->isAccepting = true;
                acceptList.emplace(Priority(start.second,i));
            }else{
                // or the e-nfa's together
                curr = graph.getGraph();
                curr.second->isAccepting = true;
                acceptList.insert(Priority(curr.second,i));
                start = orExpression(curr,start);
            }

        }

        // Create the DFA Helper which will construct the neccessary states
        DFAHelper h(start, acceptList);
        uncompressedStateList.reserve(1024);// roughly 1 mb


        const vector<DFAStateHelper*> & list =h.list;

        if(verbose){
            printf("Printing %lu states \n",list.size());
        }
        for(const DFAStateHelper * helperState : list){
            State state(helperState,maxState++);

            // this is very important vvvv
            state.set('\0',STATE_SENTINEL);
            unsigned int acceptingState = getMaxPriorityState(helperState->fromState);
            state.acceptingState = acceptingState;
            uncompressedStateList.push_back(state);
            if(verbose) {
               // h->printSelf();
                state.printState();
            }
            delete helperState;
        }
        if(verbose) {
            uint byteSize = uncompressedStateList.size() * sizeof(State);
            uint kbSize = byteSize/=1024;
            uint mbSize = kbSize/=1024;
            printf("The number of states created was %lu\n "
                           "Estimated Size : %-20u bytes\n"
                           "Estimated Size : %-20u kb's\n"
                           "Estimated Size : %-20u mb's\n",uncompressedStateList.size(),byteSize,kbSize,mbSize);
            printf("Done creating states\n");

        }

    }
    // one will always be the new VPair, two will always have the end vertex
    VPair orExpression(const VPair & one, const VPair &two){
        using namespace JRegex;
        Vertex *start = Vertex::createEpsilon();
        one.second->edge->v2IsEpsilon = true;
        two.second->edge->v2IsEpsilon = true;
        start->edge->v1 = one.first;
        start->edge->v2 = two.first;
        one.second->edge->v2 = two.second;

        return VPair(start, two.second);

    }


    unsigned int getMaxPriorityState(const std::set<JRegex::Vertex *> & s){
        unsigned int toReturn = JLexx::STATE_SENTINEL;
        for(JRegex::Vertex * v : s){
            if(acceptList.find(v) != acceptList.end()) {
                if (acceptList[v] < toReturn) {
                    toReturn = acceptList[v];
                }
            }

        }

        return toReturn;
    }

    // the way you add to this list will determine
    // the precedence of the regex
    // index 0 == highest precedence



};
#endif //JLEXX_REGEXTODFABUILDER_H


