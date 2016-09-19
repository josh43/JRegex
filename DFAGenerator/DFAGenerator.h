//
// Created by joshua on 8/16/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_DFAGENERATOR_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_DFAGENERATOR_H

#include <map>
#include <limits>

#include "../NFAGenerator/EZGraph.h"
#include "../NFAGenerator/NFAGenerator.h"
#include "../Utility.h"
#include "../NFAGenerator/EpsilonNFAMatcher.h"

namespace JRegex {
    class DFAGenerator {

    private:
        const static unsigned int SENTINEL = std::numeric_limits<unsigned int>::max();
        //const static unsigned int DEAD = std::numeric_limits<unsigned int>::max()-1;
        typedef unsigned char  INFO;
         const static unsigned char    REGULAR=1;
         const static unsigned char    START = 2;
         const static unsigned char    ACCEPTING=4;


        struct DFAStateHelper{
            INFO                                      stateType;
            unsigned int                              stateNumber;
            set<Vertex *>                             fromState;
            set<Vertex *> *                           transitions[256];
            set<unsigned char>                        transitionsUsed;
            vector<pair<unsigned char,unsigned int> > stateTransitions;

            static const unsigned char HELPER_EPSILON = (1<<8) -1;

            ~DFAStateHelper(){
                for(int i = 0 ; i < 256;i++){
                    if(transitions[i]) {
                        delete transitions[i];

                    }
                }
            }
            DFAStateHelper(){
                //toState = nullptr;
                stateType = REGULAR;
                for(int i = 0 ; i < 256 ; i ++){transitions[i] = nullptr;}
            }

            void checkAcceptingAgainst(Vertex * v){
                if(fromState.find(v) != fromState.end()){
                    stateType |= (unsigned char)ACCEPTING;
                }
            }

            static set<Vertex *> move(DFAStateHelper & helper, unsigned char c){
                if(!helper.transitions[c]){
                    for(Vertex * v : helper.fromState){
                        if(v->edge->canTransitionOn(c)){
                            if(!helper.transitions[c]) {
                                helper.transitions[c] = new set<Vertex *>();
                            }
                            if(v->edge->v1) {
                                helper.transitions[c]->insert(v->edge->v1);
                            }

                        }
                    }
                }
                if(helper.transitions[c]==nullptr){
                    return set<Vertex *>();
                }
                set<Vertex *> final;
                for(Vertex * v : *helper.transitions[c]){
                    set<Vertex *> res = EpsilonNFAMatcher::epsilonClosure(v);
                    final.insert(res.begin(),res.end());
                }
                helper.transitions[c]->insert(final.begin(),final.end());
                helper.transitionsUsed.insert(c);
                return *helper.transitions[c];
            }

            void printSelf() const{
                printf("@stateType : %c\n",this->stateType);
                printf("@Printing Transitions\n");
                for(unsigned char c : transitionsUsed) {
                    printf(ANSI_COLOR_BLUE "\t@transition[%c]\n" ANSI_COLOR_RESET, c);
                }
                if(stateTransitions.size()){
                    printf("@START StateTransitions\n\n");
                    for(uint i = 0; i < stateTransitions.size(); i ++){
                        if(i +10 % 10 == 0){
                            printf("\n");
                        }
                        printf("T['%c']=%-10d ",stateTransitions[i].first,stateTransitions[i].second);
                    }
                    printf("@END   StateTransitions\n\n");
                }
                printf("@End \n");
            }

        };

        struct State{
            unsigned char info[4];//    4 bytes used for padding
            // but first byte will be for INFO next 3 for action
            unsigned int stateNumber; // 4 bytes
            unsigned int jump[256];  // 1024 bytes


            unsigned int operator[](unsigned char index){return jump[index];}
            State(const DFAStateHelper * stateHelper,unsigned int stateNo){
                this->stateNumber = stateHelper->stateNumber;
                info[0] = stateHelper->stateType;
                info[1] = info[2] = info[3] = REGULAR;
                for(int i = 0; i < 256; i ++){
                    jump[i] = SENTINEL;
                }
                for(auto pair : stateHelper->stateTransitions){
                    jump[pair.first] = pair.second;
                }
            }
            State(unsigned int stateNo){
                info[0] = info[1] = info[2] = info[3] = REGULAR;
                stateNumber = stateNo;
                for(int i = 0; i < 256; i ++){jump[i] = SENTINEL;}
            }

            State():State(0){ }
            // want to go into something divisible by 8
        };

        vector<State>  stateList;
        vector<string> actionList;
        pair<Vertex *, Vertex *> initialNfa;
        unsigned int MAX_NUM_STATES;
        unsigned int maxAction;
        unsigned int maxState;
    public:

        ~DFAGenerator(){
        }
        // i own it
        DFAGenerator(pair<Vertex *, Vertex *> nfaState,bool debugging = false){
            initialNfa = nfaState;
            stateList.reserve(1024);// roughly 1 mb
            MAX_NUM_STATES = 1023;
            maxAction = 0;
            maxState = 0;

            vector<DFAStateHelper*> list = createDFA();
             setTransitions(list);
            for(const DFAStateHelper * h : list){
                State state(h,maxState++);
                stateList.push_back(state);
                if(debugging) {
                    h->printSelf();
                }
                delete h;
            }


        }
        bool match(string s,bool verbose = true,bool shortestMatch = false){
            unsigned int curr = 0;
            unsigned int start = 0;
            unsigned int lastState =0;
            int lastAcceptingState = -1;
            bool toReturn = false;
            s.insert(s.begin(),'\n');
            s.push_back('\n');
            while(start  < s.length()){
               do{
                   lastState = (stateList[lastState][s[curr++]]);
                   if(lastState != SENTINEL && stateList[lastState].info[0] & ACCEPTING){
                       lastAcceptingState = lastState;
                       if(shortestMatch){
                           lastState = SENTINEL;
                       }
                       toReturn =  true;
                       if(verbose){
                           printf("Matched : %s \n",s.substr(start,curr-start).c_str());
                       }

                   }
               }while(lastState != SENTINEL);

                if(lastAcceptingState != -1){
                    printf("Matched : %s \n",s.substr(start,(curr-1)-start).c_str());

                }
                lastAcceptingState = -1;
                lastState = 0;
                start++;
                curr = start;
            }

            return toReturn;
        }
    private:
        void setTransitions(const vector<DFAStateHelper *> & list,bool safeChecker = true){
            typedef set<Vertex *>  SVP;
            map<set<Vertex *>,unsigned int > stateMap;
            if(!list.size()){throw std::invalid_argument("Illegal DFA needs to be greater than size 0!!");}
            unsigned int index = 0;
            if(safeChecker) {
                for (unsigned int i = 0; i < list.size(); i++) {
                    for(unsigned int j = i +1; j < list.size(); j++){
                        if(list[i] == list[j]){
                            throw std::invalid_argument("Cannot have multiple of the same items on the list!! most likely an error on move(...)\n");
                        }
                    }
                }
            }
            for(DFAStateHelper * h : list){
                h->stateNumber = index;
                stateMap.insert(pair<SVP,unsigned int>(h->fromState, index++));
            }
            for(DFAStateHelper * h : list) {
                for(unsigned char trans: h->transitionsUsed){
                    unsigned int final = stateMap.find(*h->transitions[trans])->second;
                    h->stateTransitions.push_back(pair<unsigned char,  unsigned int>(trans,final));
                }
            }
        }
        bool isNewTransition(const vector<DFAStateHelper *> & list,const set<Vertex *> & verts){
            for(const DFAStateHelper * helper : list){
                /*
                 * Checks if the contents of lhs and rhs are equal, that is, whether lhs.size() == rhs.size()
                 * and each element in lhs has equivalent element in rhs at the same position.
                 */
                if(helper->fromState == verts){
                    return false;
                }
            }
            return true;
        }
            vector<DFAStateHelper *> createDFA(){
            vector<DFAStateHelper *> list;
            DFAStateHelper * start = new DFAStateHelper(); start->stateType = START;

            start->fromState = EpsilonNFAMatcher::epsilonClosure(this->initialNfa.first);
            start->checkAcceptingAgainst(this->initialNfa.second);

            list.push_back(start);
            stack<DFAStateHelper * > statesLeft;
            statesLeft.push(start);
            while(!statesLeft.empty()){
                DFAStateHelper * curr = statesLeft.top();
                statesLeft.pop();
                for(Vertex * v : curr->fromState){
                    set<Vertex *> verts;
                    if(dynamic_cast<BracketEdge *>(v->edge)){
                        BracketEdge * b = dynamic_cast<BracketEdge *>(v->edge);
                        for(unsigned int i = 0; i < 256; i ++) {
                            if(b->canTransitionOn(i)) {
                                verts = DFAStateHelper::move(*curr, i);
                                if(!verts.size()){continue;}
                                if(isNewTransition(list,verts)){
                                    DFAStateHelper * toPush = new DFAStateHelper(); toPush->fromState = verts;
                                    toPush->checkAcceptingAgainst(this->initialNfa.second);
                                    list.push_back(toPush);
                                    statesLeft.push(toPush);
                                }
                            }
                        }
                    } else if(v->edge->val < Edge::UNINIT ){
                        verts = DFAStateHelper::move(*curr,v->edge->val);
                        if(!verts.size()){continue;}
                        if(isNewTransition(list,verts)){
                            DFAStateHelper * toPush = new DFAStateHelper(); toPush->fromState = verts;
                            toPush->checkAcceptingAgainst(this->initialNfa.second);
                            list.push_back(toPush);
                            statesLeft.push(toPush);
                        }

                    }// else do nada
                }

            }

            return list;
        }


    };
};
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_DFAGENERATOR_H
