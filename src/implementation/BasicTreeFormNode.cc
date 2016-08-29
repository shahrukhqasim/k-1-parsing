//
// Created by shahrukhqasim on 8/22/16.
//

#include "BasicTreeFormNode.h"

std::shared_ptr<TreeFormNodeInterface> BasicTreeFormNode::getParent() {
    // TODO: Add implementation or remove
    return nullptr;
}

int BasicTreeFormNode::getChildCount() {
    return subNodes.size();
}

void BasicTreeFormNode::getChildren(std::vector<std::shared_ptr<TreeFormNodeInterface>> &children) {
    children.clear();
    for(auto i:subNodes) {
        children.push_back(i.second);
    }
}

std::shared_ptr<TreeFormNodeInterface> BasicTreeFormNode::getChild(std::string label) {
    auto iterator=subNodes.find(label);
    if(iterator==subNodes.end())
        return nullptr;
    else
        return iterator->second;
}

std::shared_ptr<TreeFormNodeInterface>
BasicTreeFormNode::addChild(std::string label, std::shared_ptr<TreeFormNodeInterface> t) {
    subNodes[label]=t;
    return t;
}

const cv::Rect &BasicTreeFormNode::getRegion() const {
    return region;
}

void BasicTreeFormNode::setRegion(const cv::Rect &region) {
    BasicTreeFormNode::region = region;
}

bool BasicTreeFormNode::isRegionDefined() const {
    return regionDefined;
}

void BasicTreeFormNode::setRegionDefined(bool regionDefined) {
    BasicTreeFormNode::regionDefined = regionDefined;
}

std::string BasicTreeFormNode::getId() {
    return id;
}

void BasicTreeFormNode::setId(const std::string &id) {
    BasicTreeFormNode::id = id;
}
