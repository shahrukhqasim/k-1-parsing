//
// Created by shahrukhqasim on 8/22/16.
//

#include <string>

#ifndef K1_PARSING_TREEFORMPROCESSOR_H
#define K1_PARSING_TREEFORMPROCESSOR_H


#include "../interfaces/FormProcessorInterface.h"
#include "../interfaces/TreeFormModelInterface.h"

class TreeFormIterator : public FormProcessorInterface<std::string>{
    std::shared_ptr<TreeFormModelInterface> formModel;
    std::string result;

    int currentIteration;
    std::shared_ptr<TreeFormNodeProcessorInterface> processor= nullptr;
public:
    const std::shared_ptr<TreeFormNodeProcessorInterface> &getProcessor() const;

    void setProcessor(const std::shared_ptr<TreeFormNodeProcessorInterface> &processor);

public:
    TreeFormIterator(std::shared_ptr<TreeFormModelInterface> formModel);
public:
    virtual bool processForm(std::shared_ptr<RawFormInterface> anInterface) override;
    virtual bool getResult(std::string result) override;
private:
    void recursiveCall(std::shared_ptr<TreeFormNodeInterface>node);
};


#endif //K1_PARSING_TREEFORMPROCESSOR_H
