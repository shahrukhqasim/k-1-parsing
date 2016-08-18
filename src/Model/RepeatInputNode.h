
#include "InputNode.h"

#ifndef K_1_PARSING2_REPEATINPUTNODE_H
#define K_1_PARSING2_REPEATINPUTNODE_H


class RepeatInputNode : public InputNode{
public:
    RepeatInputNode(InputType inputType);

    std::string dataCoordinates;



    /**
     * Sub-nodes of this repeat input. Represented by a hash map from Point to the Node
     * Key = Cartesian coordiante of the the repeat input entry number
     * Node = theNode it refers to, which will be InputNode
     *
     * WARNING! Do not use subNodes with it it will not output correctly
     *
     */
    std::unordered_map<std::string, std::shared_ptr<Node>>repeatInputSubNodes;
};


#endif //K_1_PARSING2_REPEATINPUTNODE_H
