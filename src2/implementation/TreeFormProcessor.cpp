//
// Created by shahrukhqasim on 8/22/16.
//

#include "TreeFormIterator.h"

bool TreeFormIterator::getResult(std::string result) {
    return false;
}

TreeFormIterator::TreeFormIterator(std::shared_ptr<TreeFormModelInterface> formModel) : formModel(formModel) {

}

bool TreeFormIterator::processForm(std::shared_ptr<RawFormInterface> anInterface) {
    if(processor== nullptr) {
        std::cerr<<"Error: Processor not set in iterator"<<std::endl;
        return false;
    }

    std::shared_ptr<TreeFormNodeInterface>rootNode=formModel->constructRoot();

    for(int i=0;i<formModel->getIterations();i++) {
        currentIteration=i;
        recursiveCall(rootNode);
    }

    return false;
}

void TreeFormIterator::recursiveCall(std::shared_ptr<TreeFormNodeInterface> node) {
    bool childrenDone=false;

    processor->process(node,formModel,currentIteration,childrenDone);

    if(!childrenDone) {
        std::vector<std::shared_ptr<TreeFormNodeInterface>>children;
        node->getChildren(children);

        std::for_each(children.begin(),children.end(), [&] (std::shared_ptr<TreeFormNodeInterface>currentChild){
            recursiveCall(currentChild);
        });
    }
}

const std::shared_ptr<TreeFormNodeProcessorInterface> &TreeFormIterator::getProcessor() const {
    return processor;
}

void TreeFormIterator::setProcessor(const std::shared_ptr<TreeFormNodeProcessorInterface> &processor) {
    TreeFormIterator::processor = processor;
}


