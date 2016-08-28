//
// Created by shahrukhqasim on 8/22/16.
//

#ifndef K1_PARSING_TREEFORMPROCESSOR_H
#define K1_PARSING_TREEFORMPROCESSOR_H
#include "../interfaces/FormProcessorInterface.h"
#include <string>
#include "../../json/json/json.h"
#include <unordered_map>
#include "../interfaces/TreeFormModelInterface.h"
#include "TreeFormNodeProcessor.h"

class TreeFormProcessor : public FormProcessorInterface<Json::Value>{
    std::shared_ptr<TreeFormModelInterface> formModel;
    std::string result;
    int currentIteration;
    std::shared_ptr<RawFormInterface> form;
    std::shared_ptr<TreeFormNodeProcessor> processor= nullptr;
public:
    const std::shared_ptr<TreeFormNodeProcessorInterface> &getProcessor() const;

public:
    void setProcessor(const std::shared_ptr<TreeFormNodeProcessor> &processor);
    TreeFormProcessor(std::shared_ptr<TreeFormModelInterface> formModel);
public:
    virtual bool processForm(std::shared_ptr<RawFormInterface> anInterface) override;
    virtual bool getResult(Json::Value& result) override;

    cv::Mat getCheckboxesImage();
    cv::Mat getDivisionImage();
    cv::Mat getFieldsImage();
private:
    bool processed=false;
    Json::Value fieldsResult;
    int lastIndexJson=0;
    std::shared_ptr<TreeFormNodeInterface> root;

    void recursiveResultConvert(std::shared_ptr<TreeFormNodeInterface>currentNode);

    bool recursiveCall(std::shared_ptr<TreeFormNodeInterface>node);

    /**
     * Merges words from words into elemBoxes
     *
     * @param[in] words represent the input words
     * @param[out] elemBoxes represent the resultant elements
     */
    void mergeWordBoxes(const std::vector<TextualData>&words, std::vector<TextualData>&elemBoxes);
};


#endif //K1_PARSING_TREEFORMPROCESSOR_H
