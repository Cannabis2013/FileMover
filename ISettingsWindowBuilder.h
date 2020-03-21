#ifndef IUICOMPONENTBUILDER_H
#define IUICOMPONENTBUILDER_H

template<class TSettingsWindowInterface, class TSettingsWindowParameter>
class ISettingsWindowBuilder
{
public:
    virtual TSettingsWindowInterface *buildSettingsWindow(TSettingsWindowParameter *parameter) = 0;

};

#endif // IUICOMPONENTBUILDER_H
