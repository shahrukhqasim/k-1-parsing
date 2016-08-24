//
// Created by shahrukhqasim on 8/22/16.
//

#ifndef K1_PARSING_INPUTTREEFORMNODE_H
#define K1_PARSING_INPUTTREEFORMNODE_H


#include "BasicTreeFormNode.h"

class InputTreeFormNode : public BasicTreeFormNode{
public:
    enum InputType{
        INPUT_ALPHA_NUMERIC, /** If the input is alpha numeric */
        INPUT_NUMERIC, /** If the input is only numeric */
        INPUT_CHECKBOX /** If the input is checkbox */
    };

private:
    std::string descriptiveName;
    InputType  inputType;
    std::string data="";
public:
    InputTreeFormNode(InputType inputType);

public:
    const std::string &getDescriptiveName() const;

    void setDescriptiveName(const std::string &descriptiveName);

    InputType getInputType() const;

    const std::string &getData() const;

    void setData(const std::string &data);
};


#endif //K1_PARSING_INPUTTREEFORMNODE_H
