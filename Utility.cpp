//
// Created by joshua on 8/15/16.
//
#include "Utility.h"

using namespace std;
void printError(string errorMessage,string str,char expected,int hint){

    int index = str.find(expected,hint);
    printf("%s\n",errorMessage.c_str());
    printf("%s\n",str.c_str());
    printf("%*c\n",index + 1,'^');

}
void stdinToString( std::string & buffer,bool verbose) {

    buffer = "";
    buffer.resize(512 * 40);
    char buff[512];

    memset(buff,0,sizeof(buff));

    uint totalReadSize = 0;
    uint readSize =0;
    while((readSize = read(0,buff,sizeof(buff))) > 0){
        buffer.append(buff,readSize);
        memset(buff,0,sizeof(buff));
        totalReadSize+=readSize;
        if(totalReadSize > 512 * 40){
            printf("Error cannot read more than 512 * 40 from stdin\n");
        }
        readSize = 0;

    }

    buffer.push_back('\0');



    if(verbose){
        printf("%s \n End reading from stdin\n",buffer.c_str());
    }



}
