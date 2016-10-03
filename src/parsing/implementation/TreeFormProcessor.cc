
#include "TreeFormProcessor.h"
#include "TreeFormNodeProcessor.h"
#include "RepeatInputTreeFormNode.h"

TreeFormProcessor::TreeFormProcessor(std::shared_ptr<TreeFormModelInterface> formModel) : formModel(formModel) {
}

bool TreeFormProcessor::processForm(std::shared_ptr<RawFormInterface> anInterface) {


//    if(processor== nullptr) {
//        std::cerr<<"Error: Processor not set in iterator"<<std::endl;
//        return false;
//    }

    form=anInterface;
    std::shared_ptr<TreeFormNodeInterface>rootNode=formModel->constructRoot();

    root=rootNode;

    std::vector<TextualData>words;
    std::vector<TextualData>mergedWords;
    cv::Mat image;
    anInterface->getText(words);
    anInterface->getRasterImage(image);

    //mergeWordBoxes(words,mergedWords);

    setProcessor(std::shared_ptr<TreeFormNodeProcessor>(new TreeFormNodeProcessor(image,words,std::dynamic_pointer_cast<BasicTreeFormNode>(rootNode))));

    bool error=false;
    for(int i=0;i<formModel->getIterations();i++) {
        currentIteration=i;
        std::cout<<"Running iteration "<<i<<std::endl;
        if(!recursiveCall(rootNode)) {
            error=true;
            std::cout<<"Error in iteration "<<i<<std::endl;
            break;
        }
        std::cout<<"Success in iteration "<<i<<std::endl;
    }
    processed=!error;
    return !error;
}

bool TreeFormProcessor::recursiveCall(std::shared_ptr<TreeFormNodeInterface> node) {
    bool childrenDone=false;

    if(!processor->process(node,formModel,currentIteration,childrenDone))
        return false;

    if(!childrenDone)
    {
        std::vector<std::shared_ptr<TreeFormNodeInterface>>children;
        node->getChildren(children);

        std::for_each(children.begin(),children.end(), [&] (std::shared_ptr<TreeFormNodeInterface>currentChild){
            if(!recursiveCall(currentChild))
                return false;
            return true;
        });
    }
    return true;
}

const std::shared_ptr<TreeFormNodeProcessorInterface> TreeFormProcessor::getProcessor() const {
    return processor;
}

void TreeFormProcessor::setProcessor(const std::shared_ptr<TreeFormNodeProcessor> &processor) {
    TreeFormProcessor::processor = processor;
}

void TreeFormProcessor::mergeWordBoxes(const std::vector<TextualData> &words, std::vector<TextualData> &elemBoxes) {
    // Merge the words extracted from Tesseract to obtain text-lines. The logic used for text-line extraction
    // is to merge two consecutive words if they overlap along the y-axis, and the gap between them is smaller
    // than the height of the shorter word.
    int nRects = words.size();
    bool newElem = true;
    TextualData elem, prevWord;
    for (int i = 0; i < nRects; i++) {
        TextualData currWord = words[i];
        if (!newElem) {
            int hGap = currWord.getRect().x - prevWord.getRect().x - prevWord.getRect().width;
            int hGapThresh = std::min(currWord.getRect().height, prevWord.getRect().height);
            bool vOverlap = false;
            if (((currWord.getRect().y <= prevWord.getRect().y) &&
                 (currWord.getRect().y + currWord.getRect().height > prevWord.getRect().y)) ||
                ((prevWord.getRect().y <= currWord.getRect().y) &&
                 (prevWord.getRect().y + prevWord.getRect().height > currWord.getRect().y)))
                vOverlap = true;
            bool isCheckbox=false;

            if (vOverlap && (hGap > 0) && (hGap < hGapThresh) &&!isCheckbox) {
                elem = elem | currWord;
                prevWord = currWord;
            } else {
//                if(elem.width > elem.height){
                elemBoxes.push_back(elem);
//                }
                newElem = true;
            }
        }
        if (newElem) {
            elem = currWord;
            newElem = false;
            prevWord = currWord;
            continue;
        }
    }
    elemBoxes.push_back(elem);


}

bool TreeFormProcessor::getResult(Json::Value& result) {
    lastIndexJson=0;
    foundItem= false;
    if(processed) {
        fieldsResult=Json::Value();
        recursiveResultConvert(root);
        result = fieldsResult;

        if(!foundItem) {
            std::cout<<"WARNING: Forms seems to be empty"<<std::endl;
        }

        return true;
    } else
        return false;
}

void TreeFormProcessor::recursiveResultConvert(std::shared_ptr<TreeFormNodeInterface> currentNode) {
    if (std::dynamic_pointer_cast<RepeatInputTreeFormNode>(currentNode) != nullptr) {
    }
    else if (std::dynamic_pointer_cast<InputTreeFormNode>(currentNode) != nullptr) {
        std::shared_ptr<InputTreeFormNode> iModel = std::dynamic_pointer_cast<InputTreeFormNode>(currentNode);

        Json::Value value;
        value["Id"] = iModel->getId();
        value["Name"] = iModel->getDescriptiveName();
        value["Value"] = iModel->getData();


        Json::Value region;
        region["l"] = iModel->isRegionDefined() ? iModel->getRegion().x : -1;
        region["t"] = iModel->isRegionDefined() ? iModel->getRegion().y : -1;
        region["r"] = iModel->isRegionDefined() ? iModel->getRegion().x + iModel->getRegion().width : -1;
        region["b"] = iModel->isRegionDefined() ? iModel->getRegion().y + iModel->getRegion().height : -1;

        foundItem=foundItem|iModel->isRegionDefined();

        value["Region"] = region;


        fieldsResult["Pages"][0]["Fields"][lastIndexJson++] = value;
    }

    std::vector<std::shared_ptr<TreeFormNodeInterface>> children;
    currentNode->getChildren(children);
    for(auto x:children) {
        recursiveResultConvert(x);
    }

}

cv::Mat TreeFormProcessor::getCheckboxesImage() {
    return processor->getCheckboxesImage();
}

cv::Mat TreeFormProcessor::getDivisionImage() {
    return processor->getDivisionImage();
}

cv::Mat TreeFormProcessor::getFieldsImage() {
    return processor->getInputImage();
}
