//
// Created by joshua on 8/15/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_EZTESTER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_EZTESTER_H

static void testPlusExpression();

static void testStarExpression();

static void testQuestionExpression();

static void testGetDataNode();

static void testBasicRegularExpression();

#include "../../NFAGenerator/EZGraph.h"
#include "../../Parser/RegularParser.h"
#include "../../Lexer/RegularLexer.h"
#include "../../NFAGenerator/NFAGenerator.h"

using namespace JRegex;

static void theTest(){
    testPlusExpression();
    testStarExpression();
    testQuestionExpression();
    testBasicRegularExpression();
    testGetDataNode();

}

static void testGetDataNode() {

}

static void testQuestionExpression() {
    RegularParser test(new RegularLexer("abc(ab|cd)[A-Z]*b?")); // include.out
    test.parse();
    RegularGraph graph(test.deque.regularDeque.front(),test.deque.lAnchor,test.deque.rAnchor);
    Vertex * start;
    printf("sizeof(Vertex) = %d\n",sizeof(Vertex));
    printf("sizeof(Edge) = %d\n",sizeof(Edge));

    start = graph.getGraph().first;
    // RegularGraph::specialExpression(STAR_NODE,start,end);
    vector<Vertex *> resetList;
    printf("\nBEGIN\n");
    start->dfs(start, 'z',10,resetList,true);
    for(Vertex *V : resetList){
        V->visited = false;
    }
    printf("DONE\n");
    graph.printInfo();
}

static void testStarExpression() {
    RegularParser test(new RegularLexer("(a|b)[a-z]+d")); // include.out
    test.parse();
    RegularGraph graph(test.deque.regularDeque.front(),test.deque.lAnchor,test.deque.rAnchor);
    Vertex * start;
    printf("sizeof(Vertex) = %d\n",sizeof(Vertex));
    printf("sizeof(Edge) = %d\n",sizeof(Edge));

    start = graph.getGraph().first;
   // RegularGraph::specialExpression(STAR_NODE,start,end);
    vector<Vertex *> resetList;
    printf("\nBEGIN\n");
    start->dfs(start, 'Z',10,resetList,true);
    for(Vertex *V : resetList){
        V->visited = false;
    }
    printf("DONE\n");
    graph.printInfo();

}

static void testPlusExpression() {
    RegularParser test(new RegularLexer("ab+c(ab|cd)+(a|b|c|d)+f")); // include.out
    test.parse();
    RegularGraph graph(test.deque.regularDeque.front(),test.deque.lAnchor,test.deque.rAnchor);
    Vertex * start;
    printf("sizeof(Vertex) = %d\n",sizeof(Vertex));
    printf("sizeof(Edge) = %d\n",sizeof(Edge));

    start = graph.getGraph().first;
    // RegularGraph::specialExpression(STAR_NODE,start,end);
    vector<Vertex *> resetList;
    printf("\nBEGIN\n");
    start->dfs(start, 'Z',10,resetList,true);
    for(Vertex *V : resetList){
        V->visited = false;
    }
    printf("DONE\n");
    graph.printInfo();

}

static string fileToString(const std::string & fileName){


    FILE * f = fopen(fileName.c_str(),"r");
    if(f == nullptr){printf("ERror opening file %s\n",fileName.c_str()); exit(0);}
    std::string str;
    fseek(f,0,SEEK_END);
    unsigned int fileSize = ftell(f);
    fseek(f,0,SEEK_SET);
    char * input = new char[fileSize];
    fread(input,fileSize,1,f);
    printf("Input string %s\n",str.c_str());
    std::string toReturn(input,fileSize);
    delete []input;

    return toReturn;
}
static void moreAdvanced(){



    RegularParser include(new RegularLexer("a*")); // include.out
    RegularParser boolTester(new RegularLexer("\\[[0-9]+\\]")); // include.out
    RegularParser commentTester(new RegularLexer("[A-Z]+")); // include.out
    RegularParser varNameTester(new RegularLexer("[a-z]+ [a-zA-Z]+[ ]?{[^}]*}")); // class// or namespace
    RegularParser functionNameTest(new RegularLexer("if[ ]*[(][^)]*[)][ ]*{[^}]*}")); // include.out

    include.parse(); // it parses the regular expression :)
    boolTester.parse();
    commentTester.parse();
    varNameTester.parse();
    functionNameTest.parse();
 //pair<Vertex *,Vertex *> v1 = RegularGraph::getGraphFrom(include.deque);
 //pair<Vertex *,Vertex *> v2 = RegularGraph::getGraphFrom(boolTester.deque);
 //pair<Vertex *,Vertex *> v3 = RegularGraph::getGraphFrom(commentTester.deque);
 //pair<Vertex *,Vertex *> v4 = RegularGraph::getGraphFrom(varNameTester.deque);
 //pair<Vertex *,Vertex *> v5 = RegularGraph::getGraphFrom(functionNameTest.deque);
 //string base("/Users/josh/ClionProjects/LanguageImplementationPatterns/RegularExpression/TestVSGrep/Test2/");
 //EpsilonNFA   t1(v1.first,v1.second);
 //t1.match(fileToString(base + "Test.h"),true);t1.outputToFile(base + "out/t1.out");
 //EpsilonNFA   t2(v2.first,v2.second);
 //t2.match(fileToString(base + "Test.h"),true);t2.outputToFile(base + "out/t2.out");
 //EpsilonNFA   t3(v3.first,v3.second);
 //t3.match(fileToString(base + "Test.h"),true);t3.outputToFile(base + "out/t3.out");
 //EpsilonNFA   t4(v4.first,v4.second);
 //t4.match(fileToString(base + "Test.h"),true);t4.outputToFile(base + "out/t4.out");
 //EpsilonNFA   t5(v5.first,v5.second);
 //t5.match(fileToString(base + "Test.h"),true);t5.outputToFile(base + "out/t5.out");



}
static void testBasicRegularExpression(){
   //RegularDeque regularNodeStack;
   //RegularNode * a = new RegularNode();
   //a->regularData = "a";
   //a->nodeType = DATA_NODE;
   //regularNodeStack.push(a);

   //RegularNode * b = new RegularNode();
   //b->regularData = "b";
   //b->nodeType = DATA_NODE;
   //regularNodeStack.push(b);

   //pair<Vertex *,Vertex *> v = RegularGraph::getGraphFrom(regularNodeStack);
    moreAdvanced();
}


#endif //LANGUAGEIMPLEMENTATIONPATTERNS_EZTESTER_H
