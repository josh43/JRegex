//
// Created by joshua on 8/16/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_DFAGENTESTER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_DFAGENTESTER_H

#include "../Lexer/RegularLexer.h"
#include "../Parser/RegularParser.h"
#include "DFAGenerator.h"

using namespace JRegex;
void testDFAThree(){
    RegularParser test(new RegularLexer("(a|b|c|(a*d+[a-zA-Z]+))abc?de?f")); // include.out
    test.parse(); // it parses the regular expression :)
   // pair<Vertex *,Vertex *> v1 = RegularGraph::getGraphFrom(test.deque);
//    DFAGenerator gen(v1);
}
void testDFATwo(){
    RegularParser test(new RegularLexer("(a|b)[a-z]+d")); // include.out
    test.parse(); // it parses the regular expression :)
   // pair<Vertex *,Vertex *> v1 = RegularGraph::getGraphFrom(test.deque);
    //DFAGenerator gen(v1);
    testDFAThree();
}
void testDFA(){
    RegularParser test(new RegularLexer("\\[[0-9]+\\]")); // include.out
    test.parse(); // it parses the regular expression :)
//    pair<Vertex *,Vertex *> v1 = RegularGraph::getGraphFrom(test.deque);
    //DFAGenerator gen(v1);


    testDFATwo();

}
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_DFAGENTESTER_H
