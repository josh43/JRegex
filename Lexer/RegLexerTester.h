
#include "RegularLexer.h"

void testOne(){
    RegularLexer * regular = new RegularLexer("helloWorld how are you??");
    RegularToken t;
    do{
        t = regular->nextToken();
        printf("FOUND : %s \n",t.toString().c_str());
    }while(t.token != EOF_TOKEN);

    delete regular;

}

void testTwo(){
    RegularLexer * regular = new RegularLexer("(a-z)(a(abc|adf)*z+)");
    RegularToken t;
    do{
        t = regular->nextToken();
        printf("FOUND : %s \n",t.toString().c_str());
    }while(t.token != EOF_TOKEN);

    delete regular;

}