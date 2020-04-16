#ifndef ISETTINGSDELEGATE_H
#define ISETTINGSDELEGATE_H


template<class TGeometry>
class ISettingsModel
{
public:
    virtual bool isCloseOnExitEnabled() const = 0;
    virtual bool isRuleTimerEnabled() const = 0;
    virtual bool isRulesEnabled() const = 0;
    virtual int ruleTimerInterval() const = 0;
    virtual TGeometry geometry() const = 0;
    virtual void setCloseOnExitEnabled(const bool &enable) = 0;
    virtual void setRuleTimerEnabled(const bool &enable) = 0;
    virtual void setRulesEnabled(const bool &enable) = 0;
    virtual void setRuleTimerInterval(const int &interval) = 0;
    virtual void setGeometry(const TGeometry &geometry) = 0;
};

#endif // ISETTINGSDELEGATE_H
