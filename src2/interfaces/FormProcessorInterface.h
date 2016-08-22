//
// Created by shahrukhqasim on 8/21/16.
//

#include "RawFormInterface.h"
#include "FormModelInterface.h"

#ifndef K1_PARSING_FORMPROCESSORINTERFACE_H
#define K1_PARSING_FORMPROCESSORINTERFACE_H


class FormProcessorInterface {

    virtual bool processForm(RawFormInterface)=0;

    template<typename ResultType>
    template<typename Result>
    virtual bool getResult(Result,ResultType)=0;
};


#endif //K1_PARSING_FORMPROCESSORINTERFACE_H
