//
// Created by shahrukhqasim on 8/21/16.
//

#ifndef K1_PARSING_TREEDFORMMODELINTERFACE_H
#define K1_PARSING_TREEDFORMMODELINTERFACE_H

#include <memory>
#include "FormModelInterface.h"
#include "TreeFormNodeInterface.h"

class TreeFormModelInterface : public FormModelInterface {
public:
    virtual std::shared_ptr<TreeFormNodeInterface> constructRoot()=0;
    virtual int getIterations()=0;
};


#endif //K1_PARSING_TREEDFORMMODELINTERFACE_H
