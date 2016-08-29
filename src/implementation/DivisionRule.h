#include "string"

#ifndef K1_PARSING_DIVISIONRULE_H
#define K1_PARSING_DIVISIONRULE_H


#include "../interfaces/RuleInterface.h"

class DivisionRule : public RuleInterface{
protected:
    std::string ruleKeyword;
public:
    DivisionRule(const std::string &ruleKeyword, const std::string &thisNodeId);

protected:
    std::string thisNodeId;
public:
    virtual ~DivisionRule() override;

    const std::string &getRuleKeyword() const;

    const std::string &getThisNodeId() const;

    void setThisNodeId(const std::string &thisNodeId);

    void setRuleKeyword(const std::string &ruleKeyword);
};


#endif //K1_PARSING_DIVISIONRULE_H
