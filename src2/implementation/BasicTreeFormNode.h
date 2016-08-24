//
// Created by shahrukhqasim on 8/22/16.
//
#include <string>
#include <unordered_map>
#include <unordered_set>

#ifndef K1_PARSING_BASICTREEFORMNODE_H
#define K1_PARSING_BASICTREEFORMNODE_H


#include "../interfaces/TreeFormNodeInterface.h"

class BasicTreeFormNode : public TreeFormNodeInterface {
private:
    cv::Rect region;
    std::string id;
public:
    virtual std::string getId() override;

    const cv::Rect &getRegion() const;

    void setRegion(const cv::Rect &region);

    bool isRegionDefined() const;

    void setId(const std::string &id);

    void setRegionDefined(bool regionDefined);

private:
    bool regionDefined=false;

    std::unordered_map<std::string, std::shared_ptr<TreeFormNodeInterface>>subNodes;
    std::shared_ptr<TreeFormNodeProcessorInterface> processor;

public:
    virtual std::shared_ptr<TreeFormNodeInterface> getChild(std::string label) override;

    virtual std::shared_ptr<TreeFormNodeInterface>
    addChild(std::string label, std::shared_ptr<TreeFormNodeInterface> t) override;

    virtual std::shared_ptr<TreeFormNodeInterface> getParent() override;

    virtual int getChildCount() override;

    virtual void getChildren(std::vector<std::shared_ptr<TreeFormNodeInterface>> &children) override;


    std::unordered_map<std::string, std::unordered_set<std::string>> rulesModel;
};


#endif //K1_PARSING_BASICTREEFORMNODE_H
