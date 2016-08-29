//
// Created by shahrukhqasim on 8/22/16.
//




#ifndef K1_PARSING_TREEFORMNODEPROCESSOR_INTERFACE_H
#define K1_PARSING_TREEFORMNODEPROCESSOR_INTERFACE_H

#include <memory>
#include <vector>
#include "TreeFormModelInterface.h"
#include "../TextualData.h"
#include "TreeFormNodeInterface.h"
class TreeFormNodeInterface;
class TreeFormModelInterface;
class TreeFormNodeProcessorInterface {
public:
    virtual bool process(std::shared_ptr<TreeFormNodeInterface>,std::shared_ptr<TreeFormModelInterface>,int iteration,bool&childrenDone)=0;
};


#endif //K1_PARSING_TREEFORMNODEPROCESSOR_H
