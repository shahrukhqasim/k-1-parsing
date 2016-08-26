//
// Created by shahrukhqasim on 8/26/16.
//

#include "DivisionRule.h"

#ifndef K1_PARSING_DIVISIONRULEWITHREFERENCE_H
#define K1_PARSING_DIVISIONRULEWITHREFERENCE_H


class DivisionRuleWithReference : public DivisionRule{
protected:
    std::string otherNodeId;
public:
    DivisionRuleWithReference(const std::string &ruleKeyword, const std::string &thisNodeId,
                              const std::string &otherNodeId);

    const std::string &getOtherNodeId() const;

    void setOtherNodeId(const std::string &otherNodeId);

    virtual ~DivisionRuleWithReference() override;
};

#endif //K1_PARSING_DIVISIONRULEWITHREFERENCE_H
