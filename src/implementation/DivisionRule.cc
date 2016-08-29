
#include "DivisionRule.h"

const std::string &DivisionRule::getRuleKeyword() const {
    return ruleKeyword;
}

void DivisionRule::setRuleKeyword(const std::string &ruleKeyword) {
    DivisionRule::ruleKeyword = ruleKeyword;
}

const std::string &DivisionRule::getThisNodeId() const {
    return thisNodeId;
}

void DivisionRule::setThisNodeId(const std::string &thisNodeId) {
    DivisionRule::thisNodeId = thisNodeId;
}

DivisionRule::~DivisionRule() {
}

RuleInterface::~RuleInterface() {
}


DivisionRule::DivisionRule(const std::string &ruleKeyword, const std::string &thisNodeId) : ruleKeyword(ruleKeyword),
                                                                                            thisNodeId(thisNodeId) {}
