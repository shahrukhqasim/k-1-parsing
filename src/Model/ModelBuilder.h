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

public:
    static void runModelBuilderProgram(string path);
    shared_ptr<Node> execute(string path);

    /**
     * Converts document model to JSON format recursively
     *
     * @param[out] jsonOutput represents the output JSON value
     * @param[in] model represents the input model
     */
    static void convertToJson(Json::Value& jsonOutput, const shared_ptr<Node>&model);

    /**
     * From the tree, finds the node specified by IDs hierarchy vector. First
     * element is topmost parent and last is the node itself.
     *
     * @param[in] hierarchy is the vector of hierarchy
     * @param[in] theNode is the root node
     *
     * @return The node. nullptr if node was not found
     */
    static shared_ptr<Node>findNode(const vector<string>&hierarchy, shared_ptr<Node>theNode);

private:


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
     * Processes a statement of of Form Definition Language
     *
     * @param statement
     * @param lineNumber
     */
    void processStatement(string statement, int lineNumber);

    /**
     * Creates a tree hierarchy specified by the hierarchy vector
     *
     * @param[in] hierarchy is the hierarchy vector
     * @return the last node in the hierarchy
     */
    shared_ptr<Node>createHierarchy(vector<string>&hierarchy);

    /**
     * From the tree, finds the node specified by IDs hierarchy vector. First
     * element is topmost parent and last is the node itself.
     *
     * @param[in] hierarchy is the vector of hierarchy
     *
     * @return The node. nullptr if node was not found
     */
    shared_ptr<Node>findNode(const vector<string>&hierarchy);
};


#endif //K_1_PARSING2_MODELBUILDER_H
