//
// Created by Shah Rukh Qasim on 7/25/16.
//

#include "Node.h"
#include "InputNode.h"
#include "TextNode.h"
#include <memory>
#include <iostream>
#include "../../json/json/json.h"

using namespace std;

#ifndef K_1_PARSING2_MODELBUILDER_H
#define K_1_PARSING2_MODELBUILDER_H



class ModelBuilder {

    shared_ptr<Node> document;
    shared_ptr<Node> header1;
    shared_ptr<Node> header2;
    shared_ptr<Node> part1;
    shared_ptr<Node> part2;
    shared_ptr<Node> part3;

public:
    static void start();
    void execute();
    void buildHeader1();
    void buildHeader2();
    void buildPart1();
    void buildPart2();
    void buildPart3();

    /**
     * Converts document model to JSON format recursively
     *
     * @param[out] jsonOutput represents the output JSON value
     * @param[in] model represents the input model
     */
    void convertToJson(Json::Value& jsonOutput, const shared_ptr<Node>&model);


    /**
     * Converts a cv::Point to a string
     *
     * @param p is the cv::Point to be converted
     *
     * @return the string representation of the point
     */
    string pointToString(const Point&p);


    /**
     * Sets the rule that a is below b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isBelow(shared_ptr<Node>a,shared_ptr<Node>b);

    /**
     * Sets the rule that a is above b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isAbove(shared_ptr<Node>a,shared_ptr<Node>b);

    /**
     * Sets the rule that a is left to b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isRightTo(shared_ptr<Node>a,shared_ptr<Node>b);

    /**
     * Sets the rule that a is right to b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isLeftTo(shared_ptr<Node>a,shared_ptr<Node>b);

    /**
     * Converts recursive tree ids to in-order format
     * @param node denotes the parent node to read and modify (sub nodes will be edited)
     */
    void orderIds(shared_ptr<Node>node, int currentId=1);
};


#endif //K_1_PARSING2_MODELBUILDER_H
