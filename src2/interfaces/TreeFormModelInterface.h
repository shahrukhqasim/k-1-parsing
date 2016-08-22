//
// Created by shahrukhqasim on 8/21/16.
//
#include <memory>

#include "FormModelInterface.h"
#include "TreedFormNodeInterface.h"

#ifndef K1_PARSING_TREEDFORMMODELINTERFACE_H
#define K1_PARSING_TREEDFORMMODELINTERFACE_H


class TreedFormModelInterface : public FormModelInterface {
    std::shared_ptr<TreedFormNodeInterface> getRoot();
    std::shared_ptr<TreedFormNodeInterface> getNode();
};


#endif //K1_PARSING_TREEDFORMMODELINTERFACE_H
