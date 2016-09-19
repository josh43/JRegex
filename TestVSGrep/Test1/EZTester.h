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

using namespace Easy;

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
    Vertex * start = new Vertex('a');
    Vertex * end = new Vertex(Edge::EPSILON);
    start->edge->v1 = end;
    RegularGraph::specialExpression(QUESTION_NODE,start,end);
    vector<Vertex *> resetList;
    start->dfs(start, 'z',10,resetList,true);
}

static void testStarExpression() {
    Vertex * start = new Vertex('a');
    Vertex * end = new Vertex(Edge::EPSILON);
    start->edge->v1 = end;
    RegularGraph::specialExpression(STAR_NODE,start,end);
    vector<Vertex *> resetList;
    start->dfs(start, 'z',10,resetList,true);
}

static void testPlusExpression() {
    Vertex * start = new Vertex('a');
    Vertex * end = new Vertex(Edge::EPSILON);
    start->edge->v1 = end;
    RegularGraph::specialExpression(PLUS_NODE,start,end);
    vector<Vertex *> resetList;
    start->dfs(start, 'z',10,resetList,true);

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



    RegularParser include(new RegularLexer("#include")); // include.out
    RegularParser boolTester(new RegularLexer("bool")); // include.out
    RegularParser commentTester(new RegularLexer("//[^\n]*[\n]")); // include.out
    RegularParser varNameTester(new RegularLexer("[a-zA-Z][a-zA-Z0-9]*")); // include.out
    RegularParser functionNameTest(new RegularLexer("[a-zA-Z][a-zA-Z0-9]*[(][^)]*[)]")); // include.out

    include.parse(); // it parses the regular expression :)
    boolTester.parse();
    commentTester.parse();
    varNameTester.parse();
    functionNameTest.parse();
    pair<Vertex *,Vertex *> v1 = RegularGraph::getGraphFrom(include.stack);
    pair<Vertex *,Vertex *> v2 = RegularGraph::getGraphFrom(boolTester.stack);
    pair<Vertex *,Vertex *> v3 = RegularGraph::getGraphFrom(commentTester.stack);
    pair<Vertex *,Vertex *> v4 = RegularGraph::getGraphFrom(varNameTester.stack);
    pair<Vertex *,Vertex *> v5 = RegularGraph::getGraphFrom(functionNameTest.stack);
    string base("/Users/josh/ClionProjects/LanguageImplementationPatterns/RegularExpression/TestVSGrep/");
    EpsilonNFA   t1(v1.first,v1.second);
    t1.match(fileToString(base + "Test.h"),true);t1.outputToFile(base + "out/t1.out");
    EpsilonNFA   t2(v2.first,v2.second);
    t2.match(fileToString(base + "Test.h"),true);t2.outputToFile(base + "out/t2.out");
    EpsilonNFA   t3(v3.first,v3.second);
    t3.match(fileToString(base + "Test.h"),true);t3.outputToFile(base + "out/t3.out");
    EpsilonNFA   t4(v4.first,v4.second);
    t4.match(fileToString(base + "Test.h"),true);t4.outputToFile(base + "out/t4.out");
    EpsilonNFA   t5(v5.first,v5.second);
    t5.match(fileToString(base + "Test.h"),true);t5.outputToFile(base + "out/t5.out");



}
static void testBasicRegularExpression(){
   //RegularNodeStack regularNodeStack;
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
