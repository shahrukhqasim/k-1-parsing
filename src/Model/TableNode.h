//
// Created by shahrukhqasim on 7/25/16.
//
#include "Node.h"

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

};


#endif //K_1_PARSING2_TABLENODE_H