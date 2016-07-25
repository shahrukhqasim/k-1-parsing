//
// Created by Shah Rukh Qasim on 7/25/16.
//

#include "Node.h"

#ifndef K_1_PARSING2_TEXTNODE_H
#define K_1_PARSING2_TEXTNODE_H


class TextNode : public Node{
public:
    /**
     * The text of this node
     */
    string text;

    /**
     * Constructs this node
     *
     * @param[in] text represents the text to initialize with
     *
     */
    TextNode(const string& text);

};


#endif //K_1_PARSING2_TEXTNODE_H
