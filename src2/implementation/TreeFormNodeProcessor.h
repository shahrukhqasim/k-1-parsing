//
// Created by shahrukhqasim on 8/22/16.
//

#ifndef K1_PARSING_TREEFORMNODEPROCESSOR_H
#define K1_PARSING_TREEFORMNODEPROCESSOR_H


#include "../interfaces/TreeFormNodeProcessorInterface.h"
#include "../interfaces/RawFormInterface.h"
#include "BasicTreeFormNode.h"
#include "TreeFormModel.h"

class TreeFormNodeProcessor : TreeFormNodeProcessorInterface{
private:
    std::shared_ptr<RawFormInterface> form;
    std::vector<TextualData>text;
    std::vector<TextualData>takenText;
    cv::Mat image;
    std::shared_ptr<BasicTreeFormNode>root; // Check this thoroughly
public:


    void setForm(const std::shared_ptr<RawFormInterface> &form);
    virtual bool process(std::shared_ptr<TreeFormNodeInterface> ptr, std::shared_ptr<TreeFormModelInterface> shared_ptr,int iteration,
                         bool &childrenDone) override;
    virtual int findTextWithMinimumEditDistance(std::string textToFind);
    void convertRulesToFunctions(std::shared_ptr<BasicTreeFormNode> theNode,
                                 std::shared_ptr<TreeFormModel> model,
                                 std::shared_ptr<BasicTreeFormNode> root,
                                 std::vector<std::function<bool(const TextualData &n)>>& functionalRules);
    std::pair<std::string, cv::Rect> findTextFromFunctionalRules(std::vector<std::function<bool(const TextualData &n)>> functionalRules);
    std::vector<TextualData> findVectorizedTextFromFunctionalRules(std::vector<std::function<bool(const TextualData &n)>> functionalRules);
    int findTextWithMinimumEditDistance(std::vector<TextualData>data,std::string textToFind);
};


#endif //K1_PARSING_TREEFORMNODEPROCESSOR_H
