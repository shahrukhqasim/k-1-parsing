

#ifndef K1_PARSING_TREEFORMNODEPROCESSOR_H
#define K1_PARSING_TREEFORMNODEPROCESSOR_H

#include "../checkboxes/checkbox.h"
#include <functional>
#include "../interfaces/TreeFormNodeProcessorInterface.h"
#include "../interfaces/RawFormInterface.h"
#include "BasicTreeFormNode.h"
#include "TreeFormModel.h"

class TreeFormNodeProcessor : public TreeFormNodeProcessorInterface{
private:

    std::vector<CCheckBox> checkboxes;
    std::vector<TextualData>text;
    std::vector<TextualData>unmergedText;
    std::vector<TextualData>takenText;
    cv::Mat image;
    std::shared_ptr<BasicTreeFormNode>root;

    std::unordered_map<std::string,cv::Rect>textDividedRegion;
    std::unordered_map<std::string,std::shared_ptr<cv::Mat>> images;

    std::vector<cv::Scalar>colorBucket={cv::Scalar(0,0,255),cv::Scalar(0,255,0),cv::Scalar(0,255,255),cv::Scalar(255,0,0),cv::Scalar(255,0,255)};

    cv::RNG rng;
    cv::Scalar randomColors[5]={cv::Scalar(0,168,102),cv::Scalar(0,0,255),cv::Scalar(255,0,0),cv::Scalar(255,0,102),cv::Scalar(128,0,255)};

public:
    TreeFormNodeProcessor(const cv::Mat& image,const std::vector<TextualData> &nonMergedWords,
                          const std::shared_ptr<BasicTreeFormNode> &root);


    void setForm(const std::shared_ptr<RawFormInterface> &form);
    virtual bool process(std::shared_ptr<TreeFormNodeInterface> ptr, std::shared_ptr<TreeFormModelInterface> shared_ptr,int iteration,
                         bool &childrenDone) override;

    cv::Mat getCheckboxesImage();
    cv::Mat getDivisionImage();
    cv::Mat getInputImage();

private:
    std::shared_ptr<TextDistanceInterface> textDistanceFinder;
    virtual int findTextWithMinimumEditDistance(std::string textToFind);

public:
    void setTextDistanceFinder(const std::shared_ptr<TextDistanceInterface> &textDistanceFinder);

private:
    void convertRulesToFunctions(std::shared_ptr<BasicTreeFormNode> theNode,
                                 std::shared_ptr<TreeFormModel> model,
                                 std::shared_ptr<BasicTreeFormNode> root,
                                 std::vector<std::function<bool(const TextualData &n)>>& functionalRules);
    std::pair<std::string, cv::Rect> findTextFromFunctionalRules(std::vector<std::function<bool(const TextualData &n)>> functionalRules);
    std::vector<TextualData> findVectorizedTextFromFunctionalRules(std::vector<std::function<bool(const TextualData &n)>> functionalRules);
    int findTextWithMinimumEditDistance(std::vector<TextualData>&text,std::string textToFind);
    int findTextWithMinimumEditDistance(std::vector<TextualData>&text,std::string textToFind, cv::Rect region);

    /**
     * Merges words from words into elemBoxes
     *
     * @param[in] words represent the input words
     * @param[out] elemBoxes represent the resultant elements
     */
    void mergeWordBoxes(const std::vector<TextualData>&words, std::vector<TextualData>&elemBoxes);


    std::shared_ptr<cv::Mat> getIterationOutputImage(std::string key);
};


#endif //K1_PARSING_TREEFORMNODEPROCESSOR_H
