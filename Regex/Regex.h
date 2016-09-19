//
// Created by joshua on 8/17/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGEX_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGEX_H
#include <string>
#include "../NFAGenerator/EpsilonNFAMatcher.h"
#include "../Parser/RegularParser.h"
#include "../NFAGenerator/NFAGenerator.h"
#include "../DFAGenerator/DFAGenerator.h"
#include "../Lexer/RegularLexer.h"


namespace JRegex {
    class Regex {
    public:

        // if you use caseSensative = false it will not affect bracket nodes :|
        static Regex * createRegex(std::string);
        Regex() = delete;
        ~Regex();
        // When chaining it is important to call methods in order
        // AKA call methods as they appearfrom top of header file to bottom of header file
        // EX regex.matchLongest().verbose().match("file.in",true).output("file.out");
        Regex & matchLongest ();
        Regex & matchShortest();
        Regex & verbose();
        Regex & match (std::string,bool isFile=false);
        void    output(std::string);
        void    output();
        bool    result();
    private:
        Regex(std::string str);
        bool verbosePrinting;
        bool shouldMatchLongest;
        bool matchedOne;
        std::string toMatch;
        FILE * out;
        EpsilonNFAMatcher * matcher;
        DFAGenerator      * dfaMatcher;
        RegularGraph      * graph;


    };
}
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGEX_H
