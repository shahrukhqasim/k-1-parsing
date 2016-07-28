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

public:
    Mapper(const vector<TextualData>&data, shared_ptr<Node>node);
    void execute();
    Rect recursiveCall(shared_ptr<Node>node);
    int findMinTextIndex(const vector<TextualData> &data, const string &textToFind);

};


#endif //K_1_PARSING2_MAPPER_H
