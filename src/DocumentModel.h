
#include "data_types/model/Node.h"
#include "data_types/model/InputNode.h"
#include "data_types/model/TextNode.h"
#include <memory>
#include <iostream>
#include "../json/json/json.h"


#ifndef K_1_PARSING2_MODELBUILDER_H
#define K_1_PARSING2_MODELBUILDER_H


/**
 * Reads an MDL file and converts it to tree-like structure for further processing
 */
class DocumentModel {

    /**
     * When the program is executed, this field will contain smart pointer to the top level node
     */
    std::shared_ptr<Node> document;

public:
    /**
     * Executes ModelParser on the MDL file path provided and returns pointer to the top level parent node which contains the complete hierarchy
     *
     * @param[in] filePath is the complete path of the MDL file
     *
     * @return smart pointer the top level node
     */
    std::shared_ptr<Node> loadModelFromFile(std::string filePath);

    /**
     * Converts document model to JSON format recursively
     *
     * @param[out] jsonOutput represents the output JSON value
     * @param[in] model represents the input model
     */
    static void convertToJson(Json::Value &jsonOutput, const std::shared_ptr<Node> &model);

    /**
     * From the tree, finds the node specified by IDs hierarchy std::vector. First
     * element is topmost parent and last is the node itself.
     *
     * @param[in] hierarchy is the std::vector of hierarchy
     * @param[in] theNode is the root node
     *
     * @return The node. nullptr if node was not found
     */
    static std::shared_ptr<Node> findNode(const std::vector<std::string> &hierarchy, std::shared_ptr<Node> theNode);

private:


    /**
     * Sets the rule that a is below b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isBelow(std::shared_ptr<Node> a, std::shared_ptr<Node> b);

    /**
     * Sets the rule that a is above b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isAbove(std::shared_ptr<Node> a, std::shared_ptr<Node> b);

    /**
     * Sets the rule that a is left to b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isRightTo(std::shared_ptr<Node> a, std::shared_ptr<Node> b);

    /**
     * Sets the rule that a is right to b in the IO model
     * @param a is first node
     * @param b is second node
     */
    void isLeftTo(std::shared_ptr<Node> a, std::shared_ptr<Node> b);


    /**
     * Processes a statement of of Form Definition Language
     *
     * @param statement
     * @param lineNumber
     */
    void processStatement(std::string statement, int lineNumber);

    /**
     * Creates a tree hierarchy specified by the hierarchy std::vector
     *
     * @param[in] hierarchy is the hierarchy std::vector
     * @return the last node in the hierarchy
     */
    std::shared_ptr<Node> createHierarchy(std::vector<std::string> &hierarchy);

    /**
     * From the tree, finds the node specified by IDs hierarchy std::vector. First
     * element is topmost parent and last is the node itself.
     *
     * @param[in] hierarchy is the std::vector of hierarchy
     *
     * @return The node. nullptr if node was not found
     */
    std::shared_ptr<Node> findNode(const std::vector<std::string> &hierarchy);
};


#endif //K_1_PARSING2_MODELBUILDER_H
