
#include "InputTreeFormNode.h"

const std::string &InputTreeFormNode::getDescriptiveName() const {
    return descriptiveName;
}

void InputTreeFormNode::setDescriptiveName(const std::string &descriptiveName) {
    InputTreeFormNode::descriptiveName = descriptiveName;
}

const std::string &InputTreeFormNode::getData() const {
    return data;
}

void InputTreeFormNode::setData(const std::string &data) {
    InputTreeFormNode::data = data;
}

InputTreeFormNode::InputTreeFormNode(InputTreeFormNode::InputType inputType) : inputType(inputType) {}

InputTreeFormNode::InputType InputTreeFormNode::getInputType() const {
    return inputType;
}
