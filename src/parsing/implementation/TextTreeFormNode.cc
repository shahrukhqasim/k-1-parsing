
#include "TextTreeFormNode.h"

const std::string &TextTreeFormNode::getText() const {
    return text;
}

void TextTreeFormNode::setText(const std::string &text) {
    TextTreeFormNode::text = text;
}

const std::string &TextTreeFormNode::getTextAssigned() const {
    return textAssigned;
}

void TextTreeFormNode::setTextAssigned(const std::string &textAssigned) {
    TextTreeFormNode::textAssigned = textAssigned;
}

TextTreeFormNode::TextTreeFormNode(const std::string &text) : text(text) {}
