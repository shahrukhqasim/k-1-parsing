#include "Node.h"
#include <unordered_map>
#include <memory>

#ifndef K_1_PARSING2_TABLENODE_H
#define K_1_PARSING2_TABLENODE_H
namespace FormParser {
/**
 * If this node is a table, this class represents that
 */
class TableNode: public Node {
public:
    /**
     * Number of rows in the node
     */
    int numRows;

    /**
     * Number of columns in the node
     */
    int numCols;

    // TODO: Fix this
    std::string data;

    /**
     * Sub-nodes of this table. Represented by a hash map from Point to the Node
     * Key = Cartesian coordiante of the the table entry number
     * Node = theNode it refers to. Headers etc will be TextNode and input fields will
     * be InputNode
     *
     * WARNING! Do not use subNodes with it it will not recognize tabular structure
     *
     */
    std::unordered_map<std::string, std::shared_ptr<Node>>tableEntries;

};
} // namespace FormParser {

#endif //K_1_PARSING2_TABLENODE_H
