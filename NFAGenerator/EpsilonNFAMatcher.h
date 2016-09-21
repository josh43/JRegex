//
// Created by joshua on 8/17/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_EPSILONNFAMATCHER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_EPSILONNFAMATCHER_H

#include "EZGraph.h"
#include "../Utility.h"
#include <string>
#include <stdexcept>
#include <limits>
namespace JRegex {
    using namespace std;
class EpsilonNFAMatcher {
    public:
        static const int O_FLAG = 0;
        static const int LINE_FLAG = 1;
        vector<string> wordMatches;
        vector<pair<int,int> > newLineLocations;
        Vertex *start;
        Vertex *end;

        EpsilonNFAMatcher(Vertex *s, Vertex *e) : start(s), end(e) {

        }
        EpsilonNFAMatcher(std::pair<Vertex *,Vertex *> e) : start(e.first), end(e.second) {

        }
        ~EpsilonNFAMatcher(){

        }
        bool match( std::string & str, bool verbose = false,bool matchLongest = true) {
            vector<pair<string, int> > matches;


            int currentIndex = 0;
            int longestMatch = -1;
            set<Vertex *>  lastClosure;
            lastClosure.insert(start);
            int startIndex = 0;

            while ((uint)startIndex < str.length()) {
                set<Vertex *> closure;
                while (canTransitionOn(str[currentIndex], lastClosure, closure)&&currentIndex < str.length()) {

                    currentIndex++;
                    //transition(closure,str[currentIndex]);
                    if (foundEnd(closure, end)) {
                            if (longestMatch < currentIndex - startIndex) {
                                longestMatch = currentIndex - startIndex;
                            }
                    }
                    lastClosure = closure;
                }

                if(longestMatch != -1){
                    matches.push_back({str.substr(startIndex, currentIndex - startIndex), startIndex});
                    currentIndex--;
                }
                longestMatch = -1;
                lastClosure = set<Vertex*>();
                lastClosure.insert(start);
                    // reset because not in a valid state
                startIndex += currentIndex - startIndex + 1;
                currentIndex = startIndex;

                //currentIndex+=
            }
            if (verbose) {

                for (pair<string, int> match : matches) {
                    printf(ANSI_COLOR_BLUE "  Match Found : %s @ %i" ANSI_COLOR_RESET " \n ", match.first.c_str(),
                           match.second);
                }
            }

            for(auto x : matches){
                wordMatches.push_back(x.first);
            }
            return matches.size();
        }





        static bool canTransitionOn(unsigned char letter, const set<Vertex *> &lastClosure, set<Vertex *> &newClosure) {
            set < Vertex * > temp = lastClosure;
            for (Vertex *v : lastClosure) {
                set < Vertex * > returned = epsilonClosure(v);
                temp.insert(returned.begin(), returned.end());
            }
            for (Vertex *v : temp) {
                v->visited = false;
            }
            bool canTransition = false;

            for (Vertex *v : temp) {
                if (v->edge->canTransitionOn(letter)) {
                    if (v->edge->v1) {
                        newClosure.insert(v->edge->v1);
                    } else {
                        // it must have been the end
                        newClosure.insert(v->edge->v2);
                    }
                    canTransition = true;
                }
            }
            temp.clear();
            for (Vertex *v : newClosure) { // out of the ones you can transition to
                // you must compute the closure for every vertex
                set < Vertex * > returned = epsilonClosure(v);
                temp.insert(returned.begin(), returned.end());
            }

            newClosure = std::move(temp);
            return canTransition;
        }

        static set<Vertex *> epsilonClosure(Vertex *theVertex) {
            set < Vertex * > verts;
            closureHelper(theVertex, verts);
            for (auto v : verts) {
                v->visited = false;
            }
            return verts;
        }

        static void closureHelper(Vertex *v, set<Vertex *> &vertList) {
            if (v == nullptr) { return; }
            if (v->visited) { return; }
            v->visited = true;
            vertList.insert(v);
            if (v->edge->v1IsEpsilon || v->edge->val == Edge::EPSILON) {
                closureHelper(v->edge->v1, vertList);
            }
            if (v->edge->v2IsEpsilon || v->edge->val == Edge::EPSILON) {
                closureHelper(v->edge->v2, vertList);
            }


        }

        bool foundEnd(set<Vertex *> &closure, Vertex *end) {
            for (auto x : closure) {
                if (x == end) {
                    return true;
                }
            }
            return false;
        }

    };


};
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_EPSILONNFAMATCHER_H
