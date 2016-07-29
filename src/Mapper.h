//
// Created by shahrukhqasim on 7/28/16.
//

#include "TextualData.h"
#include "Model/Node.h"
#include "Model/TextNode.h"
#include "Model/InputNode.h"
#include "Model/TableNode.h"

#ifndef K_1_PARSING2_MAPPER_H
#define K_1_PARSING2_MAPPER_H


class Mapper {
    vector<TextualData>data;
    vector<TextualData>taken;

    shared_ptr<Node>node;
    shared_ptr<Node>ultimateParent;

public:
    Mapper(const vector<TextualData>&data, shared_ptr<Node>node, shared_ptr<Node>ultimateParent);
    Rect recursiveCallText(shared_ptr<Node> node);
    void recursiveCallInput(shared_ptr<Node> node);
    int findMinTextIndex(const vector<TextualData> &data, const string &textToFind);
    void executeTextFields();
    void executeInputFields();

    void convertRulesToFunctions(shared_ptr<Node>theNode);

    string findTextWithRules(shared_ptr<Node>node);
};


#endif //K_1_PARSING2_MAPPER_H
