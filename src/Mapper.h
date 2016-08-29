#include "TextualData.h"
#include "Model/Node.h"
#include "Model/TextNode.h"
#include "Model/InputNode.h"
#include "Model/TableNode.h"

#ifndef K_1_PARSING2_MAPPER_H
#define K_1_PARSING2_MAPPER_H

namespace FormParser {
class Mapper {
    std::vector<TextualData>data;
    std::vector<TextualData>taken;

    std::shared_ptr<Node>node;
    std::shared_ptr<Node>ultimateParent;

    int width;
    int height;

public:
    Mapper(const std::vector<TextualData>&data, std::shared_ptr<Node>node, std::shared_ptr<Node>ultimateParent, int width, int height);
    cv::Rect recursiveCallText(std::shared_ptr<Node> node);
    void recursiveCallInput(std::shared_ptr<Node> node);
    int findMinTextIndex(const std::vector<TextualData> &data, const std::string &textToFind);
    void executeTextFields();
    void executeInputFields();

    void convertRulesToFunctions(std::shared_ptr<Node>theNode);

    std::pair<std::string, cv::Rect> findTextWithRules(std::shared_ptr<Node>node);
    std::vector<TextualData> findTextWithRulesB(std::shared_ptr<Node>node);
    static cv::Rect findRectFromRules(std::shared_ptr<Node>rModel,std::shared_ptr<Node>ultimateParent, int width, int height);

};
} // namespace FormParser

#endif //K_1_PARSING2_MAPPER_H
