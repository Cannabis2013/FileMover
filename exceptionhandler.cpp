#include "exceptionhandler.h"


void ExceptionHandler::handleException(const char *msg, const ExceptionHandler::handleMode &mode, char *throwMsg)
{
    cout << msg << endl;
    if(mode == printAndClose)
        exit(1);
    else if(mode == stallWithThrow)
        throw throwMsg;
}

void ExceptionHandler::handleException(const exception *e, const ExceptionHandler::handleMode &mode, char *throwMsg)
{
    cout << e->what() << endl;
    if(mode == printAndClose)
        exit(1);
    else if(mode == stallWithThrow)
        throw throwMsg;
}
