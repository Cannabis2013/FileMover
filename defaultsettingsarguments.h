#ifndef DEFAULTSETTINGSMODELINTERFACE_H
#define DEFAULTSETTINGSMODELINTERFACE_H

template<class TGeometry>
struct DefaultSettingsArguments
{
    bool closeOnExit;
    bool rulesEnabled;
    bool rulesTimerEnabled;
    int rulesTimerInterval;
    TGeometry geometry;
};

#endif // DEFAULTSETTINGSMODELINTERFACE_H
