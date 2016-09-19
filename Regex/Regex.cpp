//
// Created by joshua on 8/17/16.
//
#include "Regex.h"

namespace JRegex {


    Regex *Regex::createRegex(string str) {
        Regex *toReturn = nullptr;
        try {
             toReturn = new Regex(str);
        }catch(...){
            return nullptr;
        }

        return toReturn;
    }

    Regex &Regex::matchLongest() {
        shouldMatchLongest = true;
        return *this;
    }

    Regex &Regex::matchShortest() {
        shouldMatchLongest = false;
        return *this;
    }

    Regex &Regex::match(std::string str,bool isFile) {

        if (isFile) {
            FILE *file = NULL;
            file = fopen(str.c_str(), "r");
            if(file == NULL) {
                printf("Error opening file %s for writing \n", str.c_str());
                return *this;
            }

            fseek(file,0,SEEK_END);
            long unsigned int size = ftell(file);
            fseek(file,0,SEEK_SET);
            if(size > 4 * 1024 * 1024){
                printf("File is bigger than 4 gigabytes, this may not be intended, if this is ok remove this check\n @FILE : %s    @LINE  %d\n\n",__FILE__,__LINE__);
                throw std::invalid_argument("");
            }
            std::string buf;  buf.reserve(size);
            fread(&buf[0],1,size,file);
            if(matcher) {
                matchedOne = matcher->match(buf, verbosePrinting, shouldMatchLongest);
            }else{
                matchedOne = dfaMatcher->match(buf,verbosePrinting);
            }
        }else{
            if(matcher) {
                matchedOne = matcher->match(str, verbosePrinting, shouldMatchLongest);
            }else{
                matchedOne = dfaMatcher->match(str,verbosePrinting);
            }

        }
        return *this;
    }

    Regex &Regex::verbose() {
        this->verbosePrinting = true;
        return *this;
    }

    void Regex::output(std::string fileName) {
            FILE *file = fopen(fileName.c_str(), "w");
            if (file == nullptr) {
                printf("Error opening file %s for writing \n", fileName.c_str());
                return;
            }
        if(matcher) {
            for (string s : this->matcher->wordMatches) {
                // remove occurances of \n because
                fprintf(file, "%s", s.c_str());
            }
        }else{

        }
            fclose(file);

    }

    void Regex::output() {
        if(this->matcher) {
            for (string s : this->matcher->wordMatches) {
                // remove occurances of \n because
                fprintf(stdout, "%s\n", s.c_str());
            }
        }
    }

    bool Regex::result() {
        return matchedOne;
    }


    Regex::~Regex() {
        if(matcher) {
            delete matcher;
        }
        if(dfaMatcher){
            delete dfaMatcher;
        }

        if(graph) {
            delete graph;
        }
    }

    // doesnt handle brackets
    void caseInsensative(std::string & str){
        string buffer;
        buffer.reserve(str.size() *4/3);
        for(unsigned int i = 0; i < buffer.size(); i++){

                if(str[i] >= 'a' && str[i] <= 'z'){
                    buffer.push_back(buffer[i]);
                    buffer.push_back((buffer[i]-'a') + 'A');
                }else if(str[i] >= 'A' && str[i] <='Z'){
                    buffer.push_back(buffer[i]);
                    buffer.push_back((buffer[i]-'A') + 'a');
                }else{
                    buffer.push_back(buffer[i]);

                }

        }
        str = std::move(buffer);
    }
    Regex::Regex(std::string str ) {
        matchedOne = false;
        verbosePrinting = false;
        shouldMatchLongest = true;
        out = NULL;
        toMatch = str;
        RegularParser strToRegexParser(new RegularLexer(toMatch));
        strToRegexParser.parse();
        graph = new RegularGraph(strToRegexParser.deque.regularDeque.front(),strToRegexParser.deque.lAnchor,strToRegexParser.deque.rAnchor);
        // theres a bug with DFAGenerator
        //dfaMatcher = new DFAGenerator(graph->getGraph());
        matcher = new EpsilonNFAMatcher(graph->getGraph());

    }
}
