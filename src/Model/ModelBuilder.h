//
// Created by Shah Rukh Qasim on 7/25/16.
//

#include "Node.h"
#include "InputNode.h"
#include "TextNode.h"
#include <memory>

#ifndef K_1_PARSING2_MODELBUILDER_H
#define K_1_PARSING2_MODELBUILDER_H

class ModelBuilder {

    shared_ptr<Node> document;
    shared_ptr<Node> header1;
    shared_ptr<Node> header2;

public:
    static void start();
    void execute();
    void buildHeader1();
    void buildHeader2();
    void buildPart1();
    void buildPart2();
};


#endif //K_1_PARSING2_MODELBUILDER_H
