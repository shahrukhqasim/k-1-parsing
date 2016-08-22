//
// Created by shahrukhqasim on 8/21/16.
//

#ifndef K1_PARSING_TREEDFORMNODEINTERFACE_H
#define K1_PARSING_TREEDFORMNODEINTERFACE_H


#include <memory>
#include "LabelledTreeNodeInterface.h"

class TreedFormNodeInterface : public LabelledTreeNodeInterface <std::shared_ptr<TreedFormNodeInterface>>{
    
};


#endif //K1_PARSING_TREEDFORMNODEINTERFACE_H
