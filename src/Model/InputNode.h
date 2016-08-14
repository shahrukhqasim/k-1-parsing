
#include "Node.h"

#ifndef K_1_PARSING2_INPUTNODE_H
#define K_1_PARSING2_INPUTNODE_H

/**
 * If the node is an input field, this class represents it
 */
class InputNode : public Node {
public:
    /**
     * Type definition of the input type
     */
    enum InputType{
        INPUT_ALPHA_NUMERIC, /** If the input is alpha numeric */
        INPUT_NUMERIC, /** If the input is only numeric */
        INPUT_CHECKBOX /** If the input is checkbox */
    };

    /**
     * The input type of the current node
     */
    InputType  inputType;

    /**
     * The parsed data
     */
    std::string data="";

    /**
     * Constructs an InputNode
     *
     * @param[in] inputType represents the inputType to make
     */
    InputNode(InputType inputType);

    /**
     * The binded gound truth fields.
     *
     * First std::string represent the value
     * Second bool represents whether it is taken or not
     */
    std::vector<std::string>bindedGroundTruthEntries;
};


#endif //K_1_PARSING2_INPUTNODE_H
