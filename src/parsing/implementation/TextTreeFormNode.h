#ifndef K1_PARSING_TEXTTREEFORMNODE_H
#define K1_PARSING_TEXTTREEFORMNODE_H


#include "BasicTreeFormNode.h"

class TextTreeFormNode : public BasicTreeFormNode{
private:
    std::string text="";
    std::string textAssigned="";

public:
    TextTreeFormNode(const std::string &text);

public:
    const std::string &getText() const;

    void setText(const std::string &text);

    const std::string &getTextAssigned() const;

    void setTextAssigned(const std::string &textAssigned);
};


#endif //K1_PARSING_TEXTTREEFORMNODE_H
