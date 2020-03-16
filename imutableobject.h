#ifndef IMUTABLEOBJECT_H
#define IMUTABLEOBJECT_H

#include <qobject.h>
#include <iostream>

using namespace std;

/*
 * Provides system tray notification service
 * Provides the 'notify observers' service
 */

class IMutableObject
{
public:
    virtual void stateChanged() = 0; // Notify observers
};

#endif // IMUTABLEOBJECT_H

