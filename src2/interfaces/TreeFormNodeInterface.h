//
// Created by shahrukhqasim on 8/21/16.
//

#include <memory>
#include <vector>
#include "LabelledTreeNodeInterface.h"
#include "TreeFormModelInterface.h"
#include "TreeFormNodeProcessorInterface.h"

#ifndef K1_PARSING_TREEDFORMNODEINTERFACE_H
#define K1_PARSING_TREEDFORMNODEINTERFACE_H

class TreeFormNodeInterface : public LabelledTreeNodeInterface <std::shared_ptr<TreeFormNodeInterface>>{
public:
    virtual std::string getId()=0;
};


#endif //K1_PARSING_TREEDFORMNODEINTERFACE_H
