#ifndef K1_PARSING_TABLETREEFORMNODE_H
#define K1_PARSING_TABLETREEFORMNODE_H


#include "BasicTreeFormNode.h"

class TableTreeFormNode : public BasicTreeFormNode{
    int numCols;
    int numRows;
public:
    TableTreeFormNode(int numCols, int numRows);

    std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator getStartIterator();
    std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>::iterator getEndIterator();

};


#endif //K1_PARSING_TABLETREEFORMNODE_H
