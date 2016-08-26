//
// Created by shahrukhqasim on 8/21/16.
//

#include <memory>

#include "RawFormInterface.h"
#include "FormModelInterface.h"

#ifndef K1_PARSING_FORMPROCESSORINTERFACE_H
#define K1_PARSING_FORMPROCESSORINTERFACE_H

template<typename Result>
class FormProcessorInterface {
    virtual bool processForm(std::shared_ptr<RawFormInterface>)=0;
    virtual bool getResult(Result&)=0;
};


#endif //K1_PARSING_FORMPROCESSORINTERFACE_H
