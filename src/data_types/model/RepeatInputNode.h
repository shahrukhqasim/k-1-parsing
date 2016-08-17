
#include "InputNode.h"

#ifndef K_1_PARSING2_REPEATINPUTNODE_H
#define K_1_PARSING2_REPEATINPUTNODE_H


class RepeatInputNode : public InputNode{
public:
    RepeatInputNode(InputType inputType);

    std::string dataCoordinates;
};


#endif //K_1_PARSING2_REPEATINPUTNODE_H
