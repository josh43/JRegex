//
// Created by joshua on 8/11/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_NFAGENERATOR_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_NFAGENERATOR_H

#include "../Parser/RegularTreeNodes.h"
#include "EZGraph.h"
#include <stack>
#include <sys/time.h>


/*
 * This will take a Regular Expression generated parse tree and
 * construct a thompson e-NFA regexp note that it is not the most efficient thompson enfa
 */
namespace JRegex {
    using namespace std;


    class RegularGraph {
        typedef std::pair<Vertex *, Vertex *> VPair;
        typedef std::pair<RegularNode *, VPair> StackPair;
        stack<StackPair> theStack;
        VPair graph;

        unsigned int  numVertices;
        unsigned long millis;
        unsigned long micros;
        unsigned long seconds;
        bool inOrSubTree;
        bool dontDeallo;
    public:



        RegularGraph() = delete;
        RegularGraph(RegularNode *start,bool lAnchor,bool rAnchor) {
            dontDeallo = false;
            micros = millis = seconds = 0;
            numVertices = 0;

            timeval startTime, stop;
            gettimeofday(&startTime, NULL);
            createGraph(start);
            if (theStack.size() != 1) {
                throw std::invalid_argument("Error creating the graph for the expression");
            }

            this->graph = theStack.top().second;
            //if(theStack.top().first){
            //    delete theStack.top().first;
            //}

            if(lAnchor){
                VPair space = getSpacePair();
                space.second->edge->v1 = this->graph.first;
                this->graph.first = space.first;
            }
            if(rAnchor){
                VPair space = getSpacePair();
                this->graph.second->edge->v1 = space.first;
                this->graph.second= space.second;
            }
            gettimeofday(&stop, NULL);
            micros = stop.tv_usec - startTime.tv_usec;
            millis = micros / 1000;
            seconds = stop.tv_sec - startTime.tv_sec;
        }
        void dfsHelper(std::vector<Vertex *> &vList,Vertex * v){

        }
        void printGraph(){
            std::vector<Vertex *> vList;
            dfsHelper(vList,this->graph.first);
        }
        void dontDeallocate(){
            dontDeallo = true;
        }

        void printInfo() {
            printf("~~~~~~~@GRAPHINFO~~~~~~~\n");
            printf("\t@Number of Vertices created : %u\n", numVertices);
            printf("\t@sizeof(numVertices)        : %lu\n", numVertices * sizeof(Vertex));
            printf("\t\t@Creation of the NFA Took\n");
            printf("\t\t@Seconds       : %-15lu\n", seconds);
            printf("\t\t@MilliSeconds  : %-15lu\n", millis);
            printf("\t\t@MicroSeconds  : %-15lu\n", micros);
            printf("~~~~~~~@GRAPHINFO~~~~~~~\n");
        }

        pair<Vertex *, Vertex *> getGraph() {
            return graph;
        }
        ~RegularGraph(){
            if(!dontDeallo) {
                if (graph.first) {
                    graph.first->removeAll();
                    graph.first = nullptr;
                }
            }
        }

    private:

        VPair getSpacePair(){
            Vertex * end = Vertex::createEpsilon();
            Vertex * space = new Vertex(' ');
            Vertex * tab = new Vertex('\t');
            Vertex * newLine = new Vertex('\n');
            space->edge->v1 = end;
            tab->edge->v1 = end;
            newLine->edge->v1 = end;
            Vertex * firstOr = Vertex::createEpsilon();
            firstOr->edge->v1 = space;
            firstOr->edge->v2 = tab;
            Vertex * toReturn = Vertex::createEpsilon();
            toReturn->edge->v1 = firstOr;
            toReturn->edge->v2 = newLine;

//

            numVertices+=6;
            return VPair(toReturn,end);

        }
        void createGraph(RegularNode *curr) {
            if (curr == nullptr) { return; }

            createGraph(curr->left);
            createGraph(curr->right);
            StackPair toPush = createStackPairFrom(curr);
            theStack.push(toPush);
            reduceStack();
            if(curr){delete curr; curr = nullptr;};
        }

        void reduceStack(bool safeCheck = true) {
            /* for <Vertex *, Vertex *>
             *       front     secondToEnd
             * It may be the case that .first and .second == eachother
             * This is intentional, you should use .second.next to connect nodes :)
             * 
             */
            if (theStack.size() < 3) {
                return;
            }
            // else check
            StackPair top = theStack.top();
            if (safeCheck) {
                check(top.first);
            }
            unsigned int type = top.first->nodeType;
            bool created = false;
            if (type & AND_NODE) {
                created = true;
                theStack.pop();
                StackPair second = theStack.top();
                theStack.pop();
                StackPair first = theStack.top();
                theStack.pop();
                theStack.push(andExpressions(first.second, second.second));
                //delete first.first;
                //delete second.first;
                //first.first = nullptr, second.first = nullptr;
                // connect theStack.pop().pop().pop().back() -> theStack.pop().pop().front()
            } else if (type & OR_NODE) {
                created = true;
                theStack.pop();
                StackPair right = theStack.top();
                theStack.pop();
                StackPair left = theStack.top();
                theStack.pop();
                theStack.push(orExpressions(left.second, right.second));
                //delete right.first;
                //delete left.first;
                //left.first = nullptr, right.first = nullptr;

            }
            if (created) {
                StackPair top = theStack.top();
                theStack.pop();
                specialExpression(type, top.second.first, top.second.second);
                theStack.push(top);
            }else{
            }
        }

        StackPair createStackPairFrom(RegularNode *curr, bool safetyCheck = true) {
            StackPair toReturn;
            VPair p = {nullptr, nullptr};
            toReturn.first = curr;
            if (safetyCheck) {
                check(curr);
            }
            unsigned int type = curr->nodeType;
            if (type & AND_NODE || type & OR_NODE) {
                return StackPair(curr, p);
            }
            // else either bracket or data node
            if (type & DATA_NODE) {
                return StackPair(curr, getDataNodeFrom(curr));
            } else if (type & BRACKET_NODE) {
                return StackPair(curr, createBracketExpression(curr));
            } else {
                throw std::invalid_argument("\nMust be either a data,bracket, OR_NODE, AND_NODE ...\n");
            }

        }

        void check(RegularNode *node) {
            const char *err = "Error this node has conflicting types check RegularParser";
            unsigned int type = node->nodeType;
            if (type & AND_NODE) {
                if (type & OR_NODE) {
                    throw std::invalid_argument(err);
                } else if (type & DATA_NODE) {
                    throw std::invalid_argument(err);
                }
            } else if (type & OR_NODE) {
                if (type & DATA_NODE) {
                    throw std::invalid_argument(err);
                }
            }
            if (type & DATA_NODE && type & BRACKET_NODE) {
                throw std::invalid_argument(err);
            }
        }

        StackPair orExpressions(const pair<Vertex *, Vertex *> &one,
                                const pair<Vertex *, Vertex *> &two) {
            Vertex *start = Vertex::createEpsilon();
            Vertex *end = Vertex::createEpsilon();
            one.second->edge->v2IsEpsilon = true;
            two.second->edge->v2IsEpsilon = true;
            start->edge->v1 = one.first;
            start->edge->v2 = two.first;
            one.second->edge->v2 = end;
            two.second->edge->v2 = end;

            numVertices += 2;
            //return StackPair(new RegularNode(SENTINEL), std::pair<Vertex *, Vertex *>(start, end));
            return StackPair(nullptr, std::pair<Vertex *, Vertex *>(start, end));
        }

        StackPair andExpressions(const pair<Vertex *, Vertex *> &one,
                                 const pair<Vertex *, Vertex *> &two) {
            if (one.second->edge->v1 || one.second->edge->v2) {
                throw std::invalid_argument("Edge must nullptr");
            }

            one.second->edge->v1 = two.first;
            one.second->edge->v1IsEpsilon = true;
            //return StackPair(new RegularNode(SENTINEL), std::pair<Vertex *, Vertex *>(one.first, two.second));
            return StackPair(nullptr, std::pair<Vertex *, Vertex *>(one.first, two.second));
        }

        VPair orVertices(const pair<Vertex *, Vertex *> &one,
                         const pair<Vertex *, Vertex *> &two, bool isSingle = false) {
            if(isSingle){
                Vertex *start = Vertex::createEpsilon();
                Vertex *end = Vertex::createEpsilon();
                //one.second->edge->v2IsEpsilon = true;
                //two.second->edge->v2IsEpsilon = true;
                start->edge->v1 = one.first;
                start->edge->v2 = two.first;
                one.first->edge->v1 = end;
                two.first->edge->v1 = end;

                numVertices += 2;
                return VPair(start, end);
            }else {
                Vertex *start = Vertex::createEpsilon();
                Vertex *end = Vertex::createEpsilon();
                one.second->edge->v2IsEpsilon = true;
                two.second->edge->v2IsEpsilon = true;
                start->edge->v1 = one.first;
                start->edge->v2 = two.first;
                one.second->edge->v2 = end;
                two.second->edge->v2 = end;

                numVertices += 2;
                return VPair(start, end);
            }

        }

        pair<Vertex *, Vertex *> bracketOr(VPair p, Vertex * toOr){
            Vertex * newStart = Vertex::createEpsilon();
            newStart->edge->v1 = p.first;
            newStart->edge->v2 = toOr;
            toOr->edge->v1 = p.second;
            return VPair(newStart,p.second);
        };

        pair<Vertex *, Vertex *> createBracketExpression(RegularNode *node) {
            pair<Vertex *, Vertex *> toReturn = {nullptr, nullptr};


            Vertex * theEnd = Vertex::createEpsilon();
            Vertex *curr = Vertex::createEpsilon();

            delete curr->edge;
            curr->edge = new BracketEdge();
            if (node->nodeType & NEGATED_BRACKET) {
                (dynamic_cast<BracketEdge *>(curr->edge))->negated = true;
            }
            BracketEdge * bEdge =dynamic_cast<BracketEdge*>(curr->edge);
            toReturn.first = curr;
            toReturn.second = theEnd;
            curr->edge->v1 = theEnd;
            unsigned int i = 0;
            do {
                if (i + 2 < node->regularData.size()) {
                    if (node->regularData[i + 1] == '-') {
                        if (node->regularData[i] > node->regularData[i+2]) {
                            throw std::invalid_argument("You cannot have a bracket range in the form of b-a where b > a\n");
                        }
                        for(unsigned char c = node->regularData[i]; c <= node->regularData[i+2]; c++) {
                            bEdge->setTableAt(c);

                        }
                        i += 2;
                    } else {
                        bEdge->setTableAt(node->regularData[i]);
                    }
                }else {
                    bEdge->setTableAt(node->regularData[i]);
                }


                i++;
            } while (i < node->regularData.size());




            specialExpression(node->nodeType, toReturn.first, toReturn.second);


            return toReturn;

        };


        pair<Vertex *, Vertex *> getDataNodeFrom(RegularNode *node) {
            if (node == nullptr) {
                throw std::invalid_argument("\n");
            }
            if (node->regularData == "") {
                throw std::invalid_argument("data node shouldnt be blank");
            }
            Vertex *start = new Vertex((unsigned char) node->regularData.front());
            Vertex *end = Vertex::createEpsilon();
            numVertices += 2;
            start->edge->v1 = end;
            specialExpression(node->nodeType, start, end);

            return std::pair<Vertex *, Vertex *>(start, end);


        };

        void specialExpression(unsigned int type, Vertex *&start, Vertex *&end) {

            if (end == nullptr) { throw std::invalid_argument("End needs to not be nullptr!!"); }

            if (type & PLUS_NODE) {

                if (!end->edge || end->edge->v2 || end->edge->v1) {
                    throw std::invalid_argument("This should not occur");
                }

                end->edge->v1IsEpsilon = true;
                end->edge->v2IsEpsilon = true;

                end->edge->v1 = start;
                end->edge->v2 = Vertex::createEpsilon();
                end = end->edge->v2;
                numVertices++;
            } else if (type & STAR_NODE) {
                if (end->edge->v1 || end->edge->v2) {
                    throw std::invalid_argument("The End should not have any edges!!");
                }
                Vertex *newStart = Vertex::createEpsilon();
                Vertex *newEnd = Vertex::createEpsilon();

                newStart->edge->v1 = start;
                newStart->edge->v2 = newEnd;


                end->edge->v1IsEpsilon = true;
                end->edge->v2IsEpsilon = true;
                end->edge->v2 = newEnd;
                end->edge->v1 = newStart;


                start = newStart;
                end = newEnd;
                numVertices += 2;


            } else if (type & QUESTION_NODE) {
                if (end->edge->v1 || end->edge->v2) {
                    throw std::invalid_argument("The End should not have any edges!!");
                }

                Vertex *newStart = Vertex::createEpsilon();

                newStart->edge->v1 = start;
                newStart->edge->v2 = end;
                start = newStart;
                numVertices++;

            } // else do nothing

        }


    };

};

#endif //LANGUAGEIMPLEMENTATIONPATTERNS_NFAGENERATOR_H
