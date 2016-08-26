//
// Created by shahrukhqasim on 8/22/16.
//

#include "TableTreeFormNode.h"

TableTreeFormNode::TableTreeFormNode(int numCols, int numRows) : numCols(numCols), numRows(numRows) {}

std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator
TableTreeFormNode::getStartIterator() {
    return subNodes.begin();
}

std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator TableTreeFormNode::getEndIterator() {
    return subNodes.end();
}
