//
// Created by shahrukhqasim on 8/22/16.
//

#include "TableTreeFormNode.h"

TableTreeFormNode::TableTreeFormNode(int numCols, int numRows) : numCols(numCols), numRows(numRows) {}

std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator
TableTreeFormNode::getStartIterator() {
    return std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::shared_ptr<TreeFormNodeInterface>>::iterator();
}

std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator TableTreeFormNode::getEndIterator() {
    return std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::shared_ptr<TreeFormNodeInterface>>::iterator();
}
