
#include "MappingJob.h"
#include "ModelBuilder.h"

using namespace cv;
using namespace std;


MappingJob::MappingJob(const shared_ptr<Node> &ultimateParent, const string &nodeId, int width, int height)
        : ultimateParent(ultimateParent), nodeId(nodeId), width(width), height(height) {}

Rect MappingJob::map() {
    lastRect = Rect(0, 0, width, height);
    recursive(ultimateParent);
    return lastRect;
}


void MappingJob::recursive(shared_ptr<Node> node) {

    if (nodeId.find(node->id) < 0) {
//        cout<<"Not checking "<<nodeId<<" "<<theNode->id<<endl;
        return;
    }



    int left = lastRect.x, top = lastRect.y, right = lastRect.width + lastRect.x, bottom = lastRect.height + lastRect.y;

//dynamic_pointer_cast<TextNode>(node)!= nullptr
    if (node->id==nodeId) {
        for_each(node->rulesModel.begin(), node->rulesModel.end(),
                 [&](pair<string, unordered_set<string>> alpha) {
                     if (alpha.first == "is_below") {
                         shared_ptr<Node> theNode = ModelBuilder::findNode(
                                 HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                         if (theNode->regionDefined) {
                             if (top < theNode->region.y + theNode->region.height)
                                 top = theNode->region.y + theNode->region.height;
                         }
                     }

                     else if (alpha.first == "is_above") {
                         shared_ptr<Node> theNode = ModelBuilder::findNode(
                                 HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                         if (theNode->regionDefined) {
                             if (bottom > theNode->region.y)
                                 bottom = theNode->region.y;
                         }
                     }

                     else if (alpha.first == "is_right_to") {
                         shared_ptr<Node> theNode = ModelBuilder::findNode(
                                 HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                         if (theNode->regionDefined) {
                             if (left < (theNode->region.x + theNode->region.width))
                                 left = theNode->region.x + theNode->region.width;
                         }
                     }

                     else if (alpha.first == "is_left_to") {
                         shared_ptr<Node> theNode = ModelBuilder::findNode(
                                 HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                         if (theNode->regionDefined) {
                             if (right > theNode->region.x)
                                 right = theNode->region.x;
                         }
                     }
                 });
    }

    lastRect = Rect(left, top, right - left, bottom - top);

    if (node->subNodes.size() != 0) {
        for_each(node->subNodes.begin(), node->subNodes.end(), [&](pair<string, shared_ptr<Node>> current) {
            recursive(current.second);
        });
    }
}