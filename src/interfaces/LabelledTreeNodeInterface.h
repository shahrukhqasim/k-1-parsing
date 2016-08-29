#include <string>
#include <vector>

#ifndef K1_PARSING_LABELLEDTREE_H
#define K1_PARSING_LABELLEDTREE_H

template<typename T>
class LabelledTreeNodeInterface {
public:
    virtual T getParent()=0;
    virtual int getChildCount()=0;
    virtual void getChildren(std::vector<T>&children)=0;
    virtual T getChild(std::string label)=0;
    virtual T addChild(std::string label, T)=0;
};


#endif //K1_PARSING_LABELLEDTREE_H
