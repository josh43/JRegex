#include <iostream>
#include "Regex/Regex.h"
#include "DFAGenerator/RegexToDFABuilder.h"


int main(int argc, char * argv[]) {
    using namespace JRegex;
    // when using or expressions you ened to surround them with parenthesis
    //Regex * reg2 = Regex::createRegex("([a-z]+|[0-9]+)"); // this will take either a string of letters or a string of numbers
    //Regex * reg3 = Regex::createRegex("([a-z0-9]+)"); // this will take a-z or 0-9 and one or more
    //Regex * reg4 = Regex::createRegex("[\n\t ][a-zA-Z]+");
    //reg2->match("hello nd329 o3k",false).output();
    //reg3->match("hello nd329 o3k",false).output();
    //printf("Begin reg4\n");
    //// note that i pad the beginning and ending of matches with a \n
    //reg4->match("hello nd329 o3k",false).output();

    if(argc < 2){
        fprintf(stderr,"Ussage is jreg REGULAR_EXPRESSION");
        return 0;
    }else if(strcmp(argv[1],"--debug") == 0){
        RegexToDFABuilder rdfa;
        rdfa.add("//[^\n]*[\n]").create(false).minimizeStates(false);
        //rdfa.add("[a-zA-Z][a-zA-Z0-9]*").create(false).minimizeStates(false);





        FILE *file = NULL;
        file = fopen("/home/josh/ClionProjects/JRegex/Test.h", "r");
        if(file == NULL) {
            printf("Error opening file %s for writing \n", "/home/josh/ClionProjects/JRegex/Test.h");
            exit(0);
        }

        fseek(file,0,SEEK_END);
        long unsigned int size = ftell(file);
        fseek(file,0,SEEK_SET);
        if(size > 4 * 1024 * 1024){
            printf("File is bigger than 4 gigabytes, this may not be intended, if this is ok remove this check\n @FILE : %s    @LINE  %d\n\n",__FILE__,__LINE__);
            throw std::invalid_argument("");
        }
        std::string buf;  buf.resize(size+1);

        fread(&buf[0],1,size,file);
        fclose(file);
        buf[size] = '\0';




        rdfa.match(buf,false,false);
    }else{
        RegexToDFABuilder rdfa;
        rdfa.add(argv[1]).create(false).minimizeStates(false);
        std::string buf;
        stdinToString(buf,false);
        rdfa.match(buf,false,false);
    }


    return 0;
}