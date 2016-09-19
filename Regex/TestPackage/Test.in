[#include <vector>

//
// Created by joshua on 8/15/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_TEST_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_TEST_H

#endif //LANGUAGEIMPLEMENTATIONPATTERNS_TEST_H
//
// Created by joshua on 8/11/16.
//



#include <vector>
#include "../Parser/RegularTreeNodes.h"
#include <set>

//#include "DataStructures.h"

namespace JRegex {

    class Vertex;

    using namespace std;
    struct Edge {
        static const unsigned char EPSILON = (1 << 8) - 1;
        Edge(unsigned char data) {
            val = data;
            v1 = nullptr; v1IsEpsilon = false;
            v2 = nullptr; v2IsEpsilon = false;
        }
        Edge() {
            v1 = nullptr;
            v2 = nullptr;
        }

        unsigned char val;
        Vertex *v1;
        bool v1IsEpsilon;
        Vertex *v2;
        bool v2IsEpsilon;
    };

    struct Vertex {
        Vertex():Vertex(Edge::EPSILON){ }
        Vertex(unsigned char data) {
            isAccepting = visited  = false;
            edge = new Edge(data);
        }

        // pair<Vertex *, Vertex *> onTransition(unsigned char x) {
        //     for (Edge edge : edgeList) {
        //         if (edge.val == x) {
        //             return pair(edge.v1, edge.v2);
        //         } else if (edge.epsilon) {
        //             return {edge.v2, nullptr};
        //         }
        //         return {nullptr, nullptr};
        //     }
        // }

        static void printVertex( Vertex *& v){
            printf("@Vertex: %p  isAccepting = %s\n",v,v->isAccepting ? "YES" : "NO");
            printf("\t@Edge : %c",v->edge->val);
            printf("\t@Edge->v1 : %p,v->edge->v1 isEpsilon : %s\n",v->edge->v1,v->edge->v1IsEpsilon ?"YES" : "NO");
            printf("\t@Edge->v2 : %p,v->edge->v2 isEpsilon : %s\n",v->edge->v2,v->edge->v2IsEpsilon ?"YES" : "NO");
            printf("@END VERTEX\n");
        }
        static Vertex * dfs(Vertex *curr, unsigned char x, int depth, vector<Vertex *> &toReset,bool verbose = false) {

            if (!curr) { return nullptr; }
            if (curr->visited) { if(verbose){printf("Already visited %p returning \n",curr);}return nullptr; }
            if (depth < 0) { return nullptr;printf("Ran out of depth returning \n"); }
            curr->visited = true;
            toReset.push_back(curr);

            printVertex(curr);

            /*
            if(onTransition(x).first != nullptr){
                dfs(onTransition(x).first,x,depth-1,toReset);
            }
             */
            if(curr->edge == nullptr){throw std::invalid_argument("Edge was null need to initalize!");}
            if (curr->edge->val == x) {
                printf("Found x at %p returning vertex %p \n",curr,curr->edge->v1);
                Vertex::resetVerts(toReset);
                return curr->edge->v1;
            } else {
                printf("Searching at %p ->v1 %p  \n",curr,curr->edge->v1);
                Vertex *res = dfs(curr->edge->v1, depth - 1, x, toReset);
                if (res == nullptr) {
                    printf("Searching at %p ->v2 %p  \n",curr,curr->edge->v2);
                    return dfs(curr->edge->v2, depth - 1, x, toReset);
                } else {
                    return nullptr;
                }
            }
        }

        static void resetVerts(vector<Vertex *> &toReset) {
            for (Vertex *v : toReset) {
                v->visited = false;
            }
        }


        bool isAccepting;
        bool visited;
        Edge * edge;

    };

    class RegularGraph {
    public:


        static pair<Vertex *,Vertex * > getGraphFrom(RegularNodeStack &regStack) {

            stack<pair<Vertex *, Vertex * > > vertStack;
            if (regStack.isEmpty()) {
                throw std::invalid_argument("You need to at least have on character for matching a regexp");
            }
            do {
                Vertex *head = new Vertex(Edge::EPSILON);
                Vertex *curr = head;
                RegularNode *node = regStack.pop(); // the first regularnode can determine if the whole epxression is
                // star'd plus'd or questioned
                unsigned int type = node->nodeType;
                if(type & DATA_NODE){
                    // add all data to this
                    if(node->regularData != "") {
                        curr->edge->val = node->regularData.front();

                    }
                    node = node->left;
                    while(node){
                        curr->edge->v1 = new Vertex(node->regularData.front());
                        specialExpression(type,curr,curr->edge->v1);
                        curr = curr->edge->v1;
                        node = node->left;

                    }

                    curr->edge->v2 = new Vertex();
                    specialExpression(type,head,curr->edge->v2);
                    vertStack.push({head,curr->edge->v2});
                }else if(type & OR_NODE){
                    pair<Vertex *, Vertex *> last = getDataNodeFrom(node->right);
                    node = node->left;
                    while(node->nodeType & OR_NODE){
                        pair<Vertex * , Vertex *>  orNode = getDataNodeFrom(node->right);
                        last = orExpressions(last,orNode);
                        node = node->left;
                    }
                    pair<Vertex * , Vertex *>  orNode = getDataNodeFrom(node);
                    last = orExpressions(last,orNode);
                    head = last.first; curr = last.second;
                    curr = last.second;
                    specialExpression(type,head,curr);
                    vertStack.push({head,curr});
                }else{
                    throw std::invalid_argument("Must either be a data node or an OR_NODE\n");
                }


                //curr->addTransition({},)

            } while (!regStack.isEmpty());

            printf("Completion\n");
            Vertex *toReturn;
            if(vertStack.empty()){
                throw std::invalid_argument("You cannot have a null regular expression!!");
            }
            toReturn = vertStack.top().first;
            Vertex * curr = vertStack.top().second;
            vertStack.pop();
            while(!vertStack.empty()){
                pair<Vertex *, Vertex *> next = vertStack.top();
                if(curr->edge->v2){
                    throw std::invalid_argument("Cannot have the second vertex set when bridging!!");

                }
                curr->edge->v2IsEpsilon = true;
                curr->edge->v2 = next.first;
                curr = next.second;
                vertStack.pop();
            }

            return pair<Vertex *, Vertex *> (toReturn,curr);
        }

        static pair<Vertex *, Vertex *> orExpressions(pair<Vertex *, Vertex *> & one, pair<Vertex *, Vertex *> &two) {
            Vertex * start = new Vertex();start->edge->v1IsEpsilon = true; start->edge->v2IsEpsilon = true;
            Vertex * end = new Vertex();one.second->edge->v2IsEpsilon = true; two.second->edge->v2IsEpsilon = true;
            start->edge->v1 = one.first; start->edge->v2 = two.first;
            one.second->edge->v2 = end; two.second->edge->v2 = end;

            return std::pair<Vertex *, Vertex *>(start,end);

        }
        static void specialExpression(unsigned int type, Vertex *& start, Vertex *& end){
            if(end == nullptr){ throw std::invalid_argument("End needs to not be nullptr!!"); }
            if(type & PLUS_NODE){
                if(!end->edge || end->edge->v2){
                    throw std::invalid_argument("This should not occur");
                }
                end->edge->v2IsEpsilon = true;

                end->edge->v2 = start;
            }else if(type & STAR_NODE){
                if(end->edge->v1 || end->edge->v2){
                    throw std::invalid_argument("The End should not have any edges!!");
                }
                Vertex * newStart = new Vertex();
                Vertex * newEnd = new Vertex();
                newStart->edge->v2IsEpsilon = true;
                newStart->edge->v1IsEpsilon = true;

                newStart->edge->v1 = start; newStart->edge->v2 = newEnd;

                end->edge->v1IsEpsilon = true;
                end->edge->v2IsEpsilon = true;
                end->edge->v2 = newEnd;
                end->edge->v1 = newStart;

                start = newStart;
                end = newEnd;

            }else if(type & QUESTION_NODE){
                if(end->edge->v1 || end->edge->v2){
                    throw std::invalid_argument("The End should not have any edges!!");
                }
                Vertex * newStart = new Vertex();
                newStart->edge->v1IsEpsilon = true;
                newStart->edge->v2IsEpsilon = true;
                newStart->edge->v1 = start; newStart->edge->v2 = end;
                start = newStart;
            } // else do nothing

        }

        static pair<Vertex *, Vertex *> getDataNodeFrom(RegularNode * node){
            Vertex * start = new Vertex();
            Vertex * curr = start;
            if(node->regularData != "") {
                curr->edge->val = node->regularData.front();
                curr = curr->edge->v1;
            }
            node = node->left;
            /*
             *
             *     node = node->left;
                     while(node){
                         curr->edge->v1 = new Vertex(node->regularData.front());
                         specialExpression(type,curr,curr->edge->v1);
                         curr = curr->edge->v1;
                         node = node->left;

                     }
             */
            while(node && node->nodeType == DATA_NODE){
                curr->edge->v1 = new Vertex(node->regularData.front());
                specialExpression(node->nodeType,curr,curr->edge->v1);
                curr = curr->edge->v1;
                node = node->left;

            };
            if(curr == nullptr && start->edge->v1 == nullptr) {
                start->edge->v1 = new Vertex();
                curr = start->edge->v1;

            }else {
                curr->edge->v2 = new Vertex(Edge::EPSILON);
            }



            return std::pair<Vertex *, Vertex *>(start,curr);

        };

    private:
        RegularGraph() { };
    };
    class EpsilonNFAMatcher {
    public:
        Vertex * start; Vertex * end;
        EpsilonNFAMatcher(Vertex * s, Vertex * e): start(s), end(e){

        }
        bool match(std::string str, bool verbose = false){
            vector< pair<string,int > > matches;

            unsigned int currentIndex = 0;
            set<Vertex *> lastClosure;
            lastClosure.insert(start);
            unsigned int startIndex = 0;
            while(currentIndex < str.length()){
                set<Vertex *> closure;
                if(str[currentIndex]< 0){
                    str[currentIndex] =std::numeric_limits<unsigned char>::min() + (-str[currentIndex]);
                }
                if(canTransitionOn(str[currentIndex++],lastClosure,closure)) {
                    //transition(closure,str[currentIndex]);
                    if (foundEnd(closure, end)) {
                        matches.push_back({str.substr(startIndex,currentIndex - startIndex), startIndex});
                    }
                    lastClosure = closure;
                }else{// reset because not in a valid state
                    startIndex = currentIndex;
                    lastClosure.clear();
                    lastClosure.insert(start);
                }
                //currentIndex+=
            }
            if(verbose){
                for(pair<string,int> match : matches){
                    printf( ANSI_COLOR_BLUE "  Match Found : %s @ %i" ANSI_COLOR_RESET " \n " ,match.first.c_str(),match.second);
                }
            }
            return matches.size();
        }

        bool canTransitionOn(unsigned char letter, const set<Vertex *> & lastClosure, set<Vertex *> &newClosure){
            set<Vertex * > temp = lastClosure;
            for(Vertex *v : lastClosure){
                set<Vertex *> returned = epsilonClosure(v);
                temp.insert(returned.begin(),returned.end());
            }
            for(Vertex * v : temp){
                v->visited= false;
            }
            bool canTransition = false;

            for(Vertex *v : temp){
                if(v->edge->val == letter){
                    if(v->edge->v1) {
                        newClosure.insert(v->edge->v1);
                    }else{
                        // it must have been the end
                        newClosure.insert(v->edge->v2);
                    }
                    canTransition = true;
                }
            }
            temp.clear();
            for(Vertex * v : newClosure){ // out of the ones you can transition to
                // you must compute the closure for every vertex
                set<Vertex *> returned = epsilonClosure(v);
                temp.insert(returned.begin(),returned.end());
            }

            newClosure = std::move(temp);
            return canTransition;
        }
        set<Vertex *> epsilonClosure(Vertex * theVertex){
            set<Vertex *> verts;
            closureHelper(theVertex,verts);
            for(auto v : verts){
                v->visited = false;
            }
            return verts;
        }
        void closureHelper(Vertex * v,set<Vertex *> & vertList){
            if(v == nullptr){ return;}
            if(v->visited){return;}
            v->visited = true;
            vertList.insert(v);
            if(v->edge->v1IsEpsilon || v->edge->val == Edge::EPSILON){
                closureHelper(v->edge->v1,vertList);
            }
            if(v->edge->v2IsEpsilon|| v->edge->val == Edge::EPSILON){
                closureHelper(v->edge->v2,vertList);
            }


        }
        bool foundEnd(set<Vertex *> & closure, Vertex * end){
            for(auto x : closure){
                if(x == end){
                    return true;
                }
            }
            return false;
        }

    };
};
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_EZGRAPH_H
