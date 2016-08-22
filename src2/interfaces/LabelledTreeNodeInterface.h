//
// Created by shahrukhqasim on 8/21/16.
//

#include <vector>

#ifndef K1_PARSING_LABELLEDTREE_H
#define K1_PARSING_LABELLEDTREE_H

template<typename T>
class ILabelledTreeNode {
    void getParent(T& parent);
    int getChildCount();
    void getChildren(std::vector<T>&children);
};


#endif //K1_PARSING_LABELLEDTREE_H
