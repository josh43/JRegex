//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_DFABUILDERHELPER_H
#define JLEXX_DFABUILDERHELPER_H
//
// Created by joshua on 8/16/16.
//



#include <map>
#include <unordered_map>
#include <cmath>
#include "../NFAGenerator/EZGraph.h"
#include "../NFAGenerator/NFAGenerator.h"
#include "../Utility.h"
#include "../NFAGenerator/EpsilonNFAMatcher.h"

namespace JLexx {
    using namespace JRegex;
    typedef unsigned char  INFO;
    // Used to indicate a non accepting state
    const static unsigned int     STATE_SENTINEL = std::numeric_limits<unsigned int>::max();

    const static unsigned char    STATE_REGULAR=(1<<8) - 1;
    const static unsigned char    STATE_START = 2;
    const static unsigned char    STATE_ACCEPTING=4;

    /*
     * Every DFAStateHelper will have a set called fromState
     * which will indicate all the vertices at the current state
     * Ex the first DFAStateHelper will have its set = closure(EPSILON) from the initial head vertex
     */
    struct DFAStateHelper{
        static const unsigned char HELPER_EPSILON = (1<<8) -1;
        unsigned int stateNumber;
        INFO stateType;
        set<Vertex *> fromState;
        set<Vertex *> * transitions[256];
        set<unsigned char> transitionsUsed;
        vector<pair<unsigned char,unsigned int> > stateTransitions;


        ~DFAStateHelper(){
            for(int i = 0 ; i < 256;i++){
                if(transitions[i]) {
                    delete transitions[i];

                }
            }
        }
        DFAStateHelper(){
            //toState = nullptr;
            stateType = STATE_REGULAR;
            for(int i = 0 ; i < 256 ; i ++){
                transitions[i] = nullptr;
            }
        }

        // aList == acceptList, if this state has one or more Vertices in aList it is accepting
        void checkAcceptingAgainst (const std::unordered_map<Vertex *, unsigned int> & aList){
            for(Vertex *v : fromState){
                if(aList.find(v) != aList.end()){
                    this->stateType |= STATE_ACCEPTING;
                }
            }
        }

        // This will calculate move, note that it will not compare it to the set
        // of already existing states
        static set<Vertex *> move(DFAStateHelper & helper, unsigned char c){
            bool needToRecalculateClosure = false;
            // if not calculated than calculate
            if(!helper.transitions[c]){
                helper.transitions[c] = new set<Vertex *>();
                needToRecalculateClosure = true;
                for(Vertex * v : helper.fromState){
                    if(v->edge->canTransitionOn(c)){
                        if(v->edge->v1 != nullptr) {
                            helper.transitions[c]->insert(v->edge->v1);
                        }

                    }
                }
            }


            if(needToRecalculateClosure) {

                set<Vertex *> final;
                for (Vertex *v : *helper.transitions[c]) {
                    set<Vertex *> res = EpsilonNFAMatcher::epsilonClosure(v);
                    final.insert(res.begin(), res.end());
                }
                helper.transitions[c]->insert(final.begin(), final.end());
                helper.transitionsUsed.insert(c);
            }
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
    // Bread and butter, will actually be used when parsing input
    struct State{
        // basic info as in what kind of state it is
        unsigned char info;             //    4 bytes used for padding
        unsigned int acceptingState;
        unsigned int stateNumber;       // 4 bytes
        unsigned int jump[256];         // 1024 bytes


        bool isAccepting(){
            return acceptingState != STATE_SENTINEL;
        }

        const unsigned int operator[](const unsigned char &index){
            return jump[index];
        }
        // the uint is disambiguating
        State(const DFAStateHelper * stateHelper,uint ){
            this->stateNumber = stateHelper->stateNumber;
            acceptingState = STATE_SENTINEL;
            info = stateHelper->stateType;

            for(int i = 0; i < 256; i ++){
                jump[i] = STATE_SENTINEL;
            }
            for(auto pair : stateHelper->stateTransitions){
                jump[pair.first] = pair.second;
            }
        }
        State(unsigned int stateNo){
            acceptingState = STATE_SENTINEL;
            info = STATE_REGULAR;
            stateNumber = stateNo;
            for(int i = 0; i < 256; i ++){jump[i] = STATE_SENTINEL;}
        }

        State():State(0){ }
        void printStateInfoToFile(FILE *f){
            fprintf(f,"{\n");
            fprintf(f,"\t%u,%u,%u,\n",this->info,this->acceptingState,this->stateNumber);
            fprintf(f,"\t{\n\t\t");
            unsigned int numUsed = 0;
            for(unsigned int c = 0; c < 256; c++){
                if(c == 255){
                    fprintf(f,"%u",jump[c]);

                }else{
                    fprintf(f,"%u,",jump[c]);
                }
                if(((numUsed + 32) % 31 )== 0){
                    printf("\n\t\t");
                }
                numUsed++;
            }
            fprintf(f,"\n\t}\n");
            fprintf(f,"}\n");

        }
        void printState(bool printTransitionsOnly = true){
            printf("Printing State %d\n\n",this->stateNumber);
            printf("Accepting State %d\n",this->acceptingState);
            unsigned int numUsed = 0;
            for(unsigned int c = 0; c < 256; c ++){
                if(((numUsed + 11) % 10 )== 0){
                    printf("\n");
                }

                if(jump[c] == STATE_SENTINEL){
                    if(!printTransitionsOnly) {
                        printf("%c = \"%u\" , ", c, jump[c]);
                        numUsed++;
                    }

                }else {
                    if(c == '\n'){
                        printf("\\n = %u , ", jump[c]);
                    }else if(c == '\t'){
                        printf("\\t = %u , ", jump[c]);
                    }else if(c == ' '){

                        printf("SPACE = %u, ", jump[c]);

                    }else {
                        printf("%c = %u , ", c, jump[c]);
                    }
                    numUsed++;
                }
            }
            printf("End Printing State %d\n\n",this->stateNumber);
        }

        // want to go into something divisible by 8
        void set(unsigned  char c, const unsigned int i1){
            jump[c] = i1;
        }
    };

    struct MinimizedState{

        std::vector<u_char> map;
        std::vector<uint> jump;
        u_char info;
        uint acceptingState;
        uint stateNumber;
        uint spaceUsed;
        MinimizedState(const State & state){
            this->info = state.info; this->acceptingState = state.acceptingState;
            this->stateNumber = state.stateNumber;

            map.push_back(0);
            jump.push_back(state.jump[0]);
            for(u_char curr = 1; curr < 255; curr++){
                if(state.jump[curr] != state.jump[curr+1]){
                    map.push_back(curr);
                    jump.push_back(state.jump[curr]);
                }
            }
            map.push_back(255);
            jump.push_back(state.jump[255]);

            spaceUsed = map.size() + (jump.size() * sizeof(uint));

        }
        void printSelf(){
            printSavingsInfo();
            printf("Priting tables \n");
            for(uint i = 0; i < map.size();i++){
                printf("map[%u] == %c\n",i,map[i]);
                printf("Values < %c will jump[%u] == %u\n",map[i],i,jump[i]);
            }
        }
        void printSavingsInfo() {
            printf("map is       size %lu \n",map.size());
            printf("jumptable is size %lu \n",jump.size() * sizeof(uint));
            float spaceSaved  = (map.size()+ 3* sizeof(uint) + (jump.size() * sizeof(uint)))/ (1024 * 256);
            printf("Used %f/100",spaceSaved);
            printf("Priting tables \n");
        }
        u_char binSearch(u_char move,uint & debug){

            uint start = 0; uint end = map.size()- 1;
            uint half = map.size()/2;
            debug = 0;
            while(end-start > 1){
                if(map[half] < move){
                    start =half;
                }else if(map[half] > move){
                    end = half;
                }else{
                    return  half;
                }
                half = (end+start)/2;
                debug++;
            }

            while(start < end){
                if(map[start]>=move){
                    return start;
                }
                debug++;
                start++;
            }
            return start;
        }
        bool isAccepting(){
            return acceptingState != STATE_SENTINEL;
        }

        uint operator[](u_char move){
            uint debug;
            return jump[binSearch(move,debug)];
        }


    };

    /*
     * This could be made more efficient
     * I create all of the states, and keep unccessary storage to their transitions and than
     * recompute them again at the end
     * While it's not computing epislon closure and all of that
     * it is going through and connecting all of the states based on their movelist
     * you could just do this all at once


     * You could even create the whole vector<State > finalStateList during this
     * But the benefits of waiting is that you will only have to reserve size for the vector once
     * rather than having to resize it and recopy all of the struct State{} 's which has a good deal of overhead

     */
    class DFAHelper {
    private:
        pair<Vertex *, Vertex *>                           initialNfa;
        const std::unordered_map<Vertex *, unsigned int> & acceptList;

    public:
        vector<DFAStateHelper*> list;
        ~DFAHelper(){
        }
        // i own it
        DFAHelper(pair<Vertex *, Vertex *> nfaState, const std::unordered_map<Vertex *, unsigned int> &aList):acceptList(aList){
            initialNfa = nfaState;
            list = createDFA();
            setTransitions(list);
        }

    private:
       /*  Algo performe epsilon closure on the first vertex, that is state 0,
        *  add state 0 to top of stack()
        *  vector<set<Vertex *> > stateList
        *  while(stack.isNotEmpty()){
        *       State s = stack.pop();
        *       for(Vertex  v: s){
        *           set<Vertex *> moveList = s.move(v.data);
        *           if(isNewState(moveList,stateList)){ addState(stateList,moveList) }
        *           uint stateNumber =  getStateNo(moveList);
        *           s.moveOn(v.data,stateNumber);
        *       }
        *  }
        *
        *
        */
        vector<DFAStateHelper *> createDFA(){
            vector<DFAStateHelper *> list;
            DFAStateHelper * start = new DFAStateHelper(); start->stateType = STATE_START;
            start->fromState = EpsilonNFAMatcher::epsilonClosure(this->initialNfa.first);
            start->checkAcceptingAgainst(acceptList);

            list.push_back(start);
            stack<DFAStateHelper * > statesLeft;
            statesLeft.push(start);
            while(!statesLeft.empty()){
                DFAStateHelper * curr = statesLeft.top();
                statesLeft.pop();
                for(Vertex * v : curr->fromState){
                    set<Vertex *> verts;
                    // a bracket edge can have range values like [A-Za-z0-9]
                    // and it can be negated so just check every possible value
                    // For things like [a-z] this could be made simpler
                    // however on more complex things like[a-z0-9A-Z] its harder to determine where to break things up
                    // and on [^a-zA-Z0~] it becomes very hard
                    if(dynamic_cast<BracketEdge *>(v->edge)){
                        BracketEdge * b = dynamic_cast<BracketEdge *>(v->edge);


                        for(unsigned int i = 0; i < 256; i ++) {
                            if(b->canTransitionOn(i)) {
                                verts = DFAStateHelper::move(*curr, i);
                                if(!verts.size()){
                                    continue;
                                }
                                if(isNewTransition(list,verts)){
                                    DFAStateHelper * toPush = new DFAStateHelper(); toPush->fromState = verts;
                                    toPush->checkAcceptingAgainst(acceptList);
                                    list.push_back(toPush);
                                    statesLeft.push(toPush);
                                }

                            }
                        }
                    } else if(v->edge->val < Edge::UNINIT ){
                        verts = DFAStateHelper::move(*curr,v->edge->val);
                        if(!verts.size()){
                            continue;
                        }
                        if(isNewTransition(list,verts)){
                            DFAStateHelper * toPush = new DFAStateHelper(); toPush->fromState = verts;
                            toPush->checkAcceptingAgainst(acceptList);
                            list.push_back(toPush);
                            statesLeft.push(toPush);
                        }

                    }// else do nada
                }

            }

            return list;
        }
        void setTransitions(const vector<DFAStateHelper *> & list,bool safeChecker = true){
            typedef set<Vertex *>  SVP;
            map<set<Vertex *>,unsigned int > stateMap;
            if(!list.size()){
                throw std::invalid_argument("Illegal DFA needs to be greater than size 0!!");
            }
            unsigned int index = 0;

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


    };


};

#endif //JLEXX_DFABUILDERHELPER_H
