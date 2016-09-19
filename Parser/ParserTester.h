//
// Created by joshua on 8/10/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_PARSERTESTER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_PARSERTESTER_H
#include "RegularParser.h"
#include "../Lexer/RegularLexer.h"

void printHelper(RegularNode * rnode){
    if(!rnode){return;}
    printf("@Printing RegularNode\n");
    printf("@Data %s\n",rnode->regularData.c_str());

    for(int i =0 ; i < print_node_names.size(); i++) {
        if(rnode->nodeType & print_node_values[i]) {
            printf("\t@Type : %s\n",print_node_names[i].c_str());
        }
    }


    printf("@left = %p\n",rnode->left);
    printf("@right = %p\n",rnode->right);
    printHelper(rnode->left);
    printHelper(rnode->right);
    printf("@End Expression Data \n");

}
void testOne(){
    vector<string > re1 = {""," ","helloworld","abc*d*a+b+.*" ,"((a(b(c)*)+)?|[a-z]+|d+)+","(az)+(bd)*","cd[^()]+def","[a-z]+*",
                           "Test [^0-9]+[0-9]+ : (PASSED|FAILED)\t .*"};/*,
                           "(a|b)","(ab|cd)","(ab*|cd+)*","(a(b|c))def[g-z]",
                           "(ab*(cd[^()]def | [a-z]+)*)", "^abcd$","^abc(ads)*&+","bcd$","^[a-z]+[A-Z]+5(abc|def)*(A|(b|(cdef)))"};*/
    vector<bool> passed;
    int i = 0;

    RegularParser reg(new RegularLexer(re1[4]));
    reg.parse();
    printHelper(reg.deque.pop());
    for(string str : re1){
        RegularParser regParser(new RegularLexer(str));
        bool valid = true;
        if(i ==6){
            printf("debug here");
        }
        try {
            regParser.parse();
        }catch(std::invalid_argument t){
            valid = false;
        }

        printf("BEGIN %s \n\n",str.c_str());
        // this will lose all the memory
        if(regParser.deque.size()) {
            RegularNode *rnode = regParser.deque.pop();
            printHelper(rnode);
        }



        printf("END %s \n\n",str.c_str());
        passed.push_back(valid);
        i++;
    }

    for(int i = 0; i < re1.size(); i ++){
        // formats correctly up to 5 digits
        printf("Test %5d :  %s  \t%s \n",i,(passed[i] == true ? "PASSED" : "FAILED"),re1[i].c_str());
    }

}
void testTwo(){
    vector<string > re1 = {"(\t|\n| )*"};/*,
                           "(a|b)","(ab|cd)","(ab*|cd+)*","(a(b|c))def[g-z]",
                           "(ab*(cd[^()]def | [a-z]+)*)", "^abcd$","^abc(ads)*&+","bcd$","^[a-z]+[A-Z]+5(abc|def)*(A|(b|(cdef)))"};*/
    vector<bool> passed;
    int i = 0;


    for(string str : re1){
        RegularParser regParser(new RegularLexer(str));
        bool valid = true;
        if(i == 12){
            printf("debug here");
        }
        try {
            regParser.parse();
        }catch(std::invalid_argument t){
            valid = false;
        }

        printf("BEGIN %s \n\n",str.c_str());
        // this will lose all the memory
        if(regParser.deque.regularDeque.size() > 1){
            throw std::invalid_argument("Size should be one");
        }
        while(!regParser.deque.regularDeque.empty()){
            RegularNode * rnode = regParser.deque.pop();
            printHelper(rnode);


        }
        printf("END %s \n\n",str.c_str());
        passed.push_back(valid);
        i++;
    }

    for(int i = 0; i < re1.size(); i ++){
        // formats correctly up to 5 digits
        printf("Test %5d :  %s  \t%s \n",i,(passed[i] == true ? "PASSED" : "FAILED"),re1[i].c_str());
    }
}
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_PARSERTESTER_H
