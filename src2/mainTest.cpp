//
// Created by shahrukhqasim on 8/21/16.
//

#include <iostream>
#include "interfaces/TreeFormModelInterface.h"
#include "implementation/TreeFormModel.h"
#include "implementation/TreeFormIterator.h"

void testFunction() {
    // Static function because if MDL is invalid, it just returns null which can't be used further in a processor
    std::shared_ptr<TreeFormModelInterface> model = TreeFormModel::constructFormModel("MDL DATA GOES HERE");

    TreeFormIterator processor(model);

    for(int i=0;i<100;i++) {
        std::shared_ptr<RawFormInterface> form=nullptr; // It is coming from somewhere
        if(processor.processForm(form)) {
            std::string result;
            processor.getResult(result);
         }


    }
}