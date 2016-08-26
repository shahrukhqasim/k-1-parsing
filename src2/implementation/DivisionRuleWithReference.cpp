//
// Created by shahrukhqasim on 8/26/16.
//

#include "DivisionRuleWithReference.h"

const std::string &DivisionRuleWithReference::getOtherNodeId() const {
    return otherNodeId;
}

void DivisionRuleWithReference::setOtherNodeId(const std::string &otherNodeId) {
    DivisionRuleWithReference::otherNodeId = otherNodeId;
}



DivisionRuleWithReference::DivisionRuleWithReference(const std::string &ruleKeyword, const std::string &thisNodeId,
                                                     const std::string &otherNodeId) : DivisionRule(ruleKeyword,
                                                                                                    thisNodeId),
                                                                                       otherNodeId(otherNodeId) {}

DivisionRuleWithReference::~DivisionRuleWithReference() {

}
