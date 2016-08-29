
#include "Node.h"

#ifndef K_1_PARSING2_TEXTNODE_H
#define K_1_PARSING2_TEXTNODE_H

namespace FormParser {
class TextNode : public Node{
public:
    /**
     * The text of this node
     */
    std::string text="";
    /**
     * The text of this node
     */
    std::string textAssigned="";

    /**
     * Constructs this node
     *
     * @param[in] text represents the text to initialize with
     *
     */
    TextNode(const std::string& text);

};
} // namespace FormParser {

#endif //K_1_PARSING2_TEXTNODE_H
