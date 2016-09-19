//
// Created by joshua on 8/11/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_EZGRAPH_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_EZGRAPH_H

#include <vector>
#include "../Parser/RegularTreeNodes.h"
#include <set>

//#include "DataStructures.h"

namespace JRegex {

    struct Vertex;

    using namespace std;

    struct Edge {
        static const unsigned char EPSILON = (1 << 8) - 1;
        static const unsigned char UNINIT = (1 << 8) - 2;

        Edge(unsigned char data) {
            val = data;
            v1 = nullptr;
            v1IsEpsilon = false;
            v2 = nullptr;
            v2IsEpsilon = false;
        }

        Edge() {
            v1 = nullptr;
            v2 = nullptr;
            val = UNINIT;
        }

        virtual ~Edge() { }

        virtual bool canTransitionOn(unsigned char c) {
            return c == val;
        }

        bool v1IsEpsilon;           // 1 byte
        bool v2IsEpsilon;           // 1
        unsigned char val;          // 1
        unsigned char padding[5];   // 5
        Vertex *v1;                 // 8
        Vertex *v2;                 // 8
        //virtual table pointer     // 8
        //___
        // 32
    };

    struct BracketEdge : public Edge {
        unsigned char end;
        bool table[256];
        bool negated;

        BracketEdge(unsigned char start, unsigned char thEnd, bool neg) : Edge(start) {
            for(int i = 0; i < 256 ; i ++){
                table[i] = false;
            }

            negated = neg;
            end = thEnd;
        }
        void setTableAt(unsigned int i){
            table[i] = true;
        }
        BracketEdge() : Edge() {
            for(int i = 0; i < 256 ; i ++){
                table[i] = false;
            }
            negated = false;

        }

        bool canTransitionOn(unsigned char c) override {
            //return negated ^ ((c >= val) && (c <= end));
            return negated ^ table[c];
        }
    };

    struct Vertex {
        Vertex() = delete;

        //:Vertex(Edge::EPSILON){ };
        Vertex(unsigned char data, bool literal = false) {
            isAccepting = visited = false;
            if (literal) {
                edge = new Edge(data);
            } else {
                switch (data) {
                    case '.': {
                        edge = new BracketEdge(0, Edge::EPSILON - 1, false);
                        return;
                    }
                    default:
                        edge = new Edge(data);
                        return;

                }
            }
        }
        ~Vertex(){
            delete edge;
        }
        void removeAll(){
            std::vector<Vertex *> vList;
            findAll(this,vList);
            if(vList.size() == 0){
                throw std::invalid_argument("Its likely that the node's have had their visited flag set to true so they must be unset!");
            }
            for(Vertex *v : vList){
                delete v;
                v = nullptr;
            }
        }

        void findAll(Vertex * curr, std::vector<Vertex *> & list){
            if (!curr) { return; }
            if (curr->visited) { return; }
            curr->visited = true;
            list.push_back(curr);

            findAll(curr->edge->v1, list);
            findAll(curr->edge->v2, list);

        }
        static void printVertex(Vertex *&v) {
            printf("@Vertex: %p  isAccepting = %s\n", v, v->isAccepting ? "YES" : "NO");
            printf("\t@Edge : %c\n", v->edge->val);
            printf("\t@Edge->v1 : %p,v->edge->v1 isEpsilon : %s\n", v->edge->v1, v->edge->v1IsEpsilon ? "YES" : "NO");
            printf("\t@Edge->v2 : %p,v->edge->v2 isEpsilon : %s\n", v->edge->v2, v->edge->v2IsEpsilon ? "YES" : "NO");
            printf("@END VERTEX\n");
        }

        static Vertex *dfs(Vertex *curr, unsigned char x, int depth, vector<Vertex *> &toReset, bool verbose = false) {

            if (!curr) { return nullptr; }
            if (curr->visited) {
                if (verbose) { printf("Already visited %p returning \n", curr); }
                return nullptr;
            }
            if (depth < 0) {
                return nullptr;
                printf("Ran out of depth returning \n");
            }
            curr->visited = true;
            toReset.push_back(curr);

            if(verbose) {
                printVertex(curr);
            }

            /*
            if(onTransition(x).first != nullptr){
                dfs(onTransition(x).first,x,depth-1,toReset);
            }
             */
            if (curr->edge == nullptr) { throw std::invalid_argument("Edge was null need to initalize!"); }
            if (curr->edge->val == x) {
                printf("Found x at %p returning vertex %p \n", curr, curr->edge->v1);
                Vertex::resetVerts(toReset);
                return curr->edge->v1;
            } else {
                printf("Searching at %p ->v1 %p  \n", curr, curr->edge->v1);
                Vertex *res = dfs(curr->edge->v1, depth - 1, x, toReset);
                if (res == nullptr) {
                    printf("Searching at %p ->v2 %p  \n", curr, curr->edge->v2);
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
        unsigned char padding[6];
        Edge *edge;

        static Vertex *createEpsilon() {
            Vertex *toReturn = new Vertex(Edge::EPSILON);
            toReturn->edge->v1IsEpsilon = true;
            toReturn->edge->v2IsEpsilon = true;
            return toReturn;
        }
    };
};



#endif //LANGUAGEIMPLEMENTATIONPATTERNS_EZGRAPH_H
