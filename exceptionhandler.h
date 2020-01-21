#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include <iostream>

using namespace std;

class ExceptionHandler
{
public:
    enum handleMode {printAndClose, onlyPrint, stallWithThrow};

    static void handleException(const char *msg,const handleMode &mode, char *throwMsg = nullptr);
    static void handleException(const std::exception *e,const handleMode &mode, char *throwMsg = nullptr);
};

typedef ExceptionHandler eHandler;

#endif // EXCEPTIONHANDLER_H
