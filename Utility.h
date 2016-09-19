#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "Lexer/Lexer.h"
using namespace std;
#define funcError() printf("%s\n line: %d file: %s",__PRETTY_FUNCTION__,__LINE__,__FILE__); printStackTrace()
//GDB PRETTY PRINTING --- type this
// set print pretty on


#define prettyPrint(xx)
//printf("@%s \n @%s \n @Line : %d \n @File : %s\n",xx,__PRETTY_FUNCTION__,__LINE__,__FILE__)

//http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
# define printStackTrace() \
    void *array[10];\
    size_t size;\
    size = backtrace(array, 10);\
    backtrace_symbols_fd(array, size, STDERR_FILENO);

#define unexpectedErr(unexp,exp) printf("@Error \n\t Unexpteded: %s \n\t expected: %s  \n\t @Function%s \n\t @Line: %d \n\t @File: %s \n@EndError\n",unexp,exp,__PRETTY_FUNCTION__,__LINE__,__FILE__); printStackTrace()

#define DEBUGGING


#define JAssert(expression,message)\
if(expression){\
    fprintf(stderr,ANSI_COLOR_RED "Assertion failed\n" ANSI_COLOR_RESET);\
    fprintf(stderr,ANSI_COLOR_RED "At function %s \n on File : %s and Line : %u\n" ANSI_COLOR_RESET,__PRETTY_FUNCTION__,__FILE__,__LINE__);\
    fprintf(stderr,ANSI_COLOR_RED "Message %s\n" ANSI_COLOR_RESET,message);\
    abort();\
}\

#define PRINTFUNCTION()\
    printf(ANSI_COLOR_MAGENTA "Entering %s \n on File : %s and Line : %u\n" ANSI_COLOR_RESET,__PRETTY_FUNCTION__,__FILE__,__LINE__)
#define ENTERING_FUNCTION()\
    printf("Entering"); PRINTFUNCTION()

#define LEAVING_FUNCTION()\
    printf("Leaving");PRINTFUNCTION()

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
void printError(string errorMessage,string str,char expected,int hint);

