
#include "TableTreeFormNode.h"

TableTreeFormNode::TableTreeFormNode(int numCols, int numRows) : numCols(numCols), numRows(numRows) {}

std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator
TableTreeFormNode::getStartIterator() {
    return subNodes.begin();
}

std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator TableTreeFormNode::getEndIterator() {
    return subNodes.end();
}

int TableTreeFormNode::getNumCols() const {
    return numCols;
}

int TableTreeFormNode::getNumRows() const {
    return numRows;
}
