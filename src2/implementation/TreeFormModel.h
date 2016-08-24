//
// Created by shahrukhqasim on 8/22/16.
//

#include <string>
#include <fstream>

#ifndef K1_PARSING_TREEFORMMODEL_H
#define K1_PARSING_TREEFORMMODEL_H


#include "../interfaces/TreeFormModelInterface.h"
#include "BasicTreeFormNode.h"

class TreeFormModel : public TreeFormModelInterface{
private:
    std::string mdlData;
    std::shared_ptr<TreeFormNodeInterface>root;

    bool processMdl();
    bool processLine(const std::string &line, int lineNumber);
    void divideLine(const std::string& line, std::vector<std::string>fields);
    std::shared_ptr<TreeFormNodeInterface> createHierarchy(std::vector<std::string>&hierarchy);
    TreeFormModel();
public:
    static std::shared_ptr<TreeFormModel> constructFormModel(std::string mdl);
    static std::shared_ptr<TreeFormModel> constructFormModel(std::ifstream mdlFile);

    virtual std::shared_ptr<TreeFormNodeInterface> constructRoot() override;

    static std::shared_ptr<TreeFormNodeInterface> getNode(std::shared_ptr<TreeFormNodeInterface> root, std::vector<std::string>hierarchy);

    virtual int getIterations() override;

    private:


    void isBelow(std::shared_ptr<BasicTreeFormNode>a, std::shared_ptr<BasicTreeFormNode>b);

    void isAbove(std::shared_ptr<BasicTreeFormNode>a, std::shared_ptr<BasicTreeFormNode>b);

    void isRightTo(std::shared_ptr<BasicTreeFormNode>a, std::shared_ptr<BasicTreeFormNode>b);

    void isLeftTo(std::shared_ptr<BasicTreeFormNode>a, std::shared_ptr<BasicTreeFormNode>b);

    void hasVerticalOverlapWith(std::shared_ptr<BasicTreeFormNode>a, std::shared_ptr<BasicTreeFormNode>b);

    void hasHorizontalOverlapWith(std::shared_ptr<BasicTreeFormNode>a, std::shared_ptr<BasicTreeFormNode>b);


};


#endif //K1_PARSING_TREEFORMMODEL_H
