//
// Created by shahrukhqasim on 7/25/16.
//
#include "Node.h"
#include <unordered_map>
#include <memory>

#ifndef K_1_PARSING2_TABLENODE_H
#define K_1_PARSING2_TABLENODE_H

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

    /**
     * Sub-nodes of this table. Represented by a hash map from Point to the Node
     * Key = Cartesian coordiante of the the table entry number
     * Node = theNode it refers to. Headers etc will be TextNode and input fields will
     * be InputNode
     *
     * WARNING! Do not use subNodes with it it will not recognize tabular structure
     *
     */
    unordered_map<string,shared_ptr<Node>>tableEntries;

};


#endif //K_1_PARSING2_TABLENODE_H