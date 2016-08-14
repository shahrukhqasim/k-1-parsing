
#include "Node.h"

#ifndef K_1_PARSING2_MAPPINGJOB_H
#define K_1_PARSING2_MAPPINGJOB_H


class MappingJob {
    std::shared_ptr<Node>ultimateParent;
    cv::Rect lastRect;
    std::string nodeId;
    int width;
    int height;
public:
    MappingJob(const std::shared_ptr<Node> &ultimateParent, const std::string &nodeId, int width, int height);

    cv::Rect map();
private:
    void recursive(std::shared_ptr<Node> node);
};


#endif //K_1_PARSING2_MAPPINGJOB_H
