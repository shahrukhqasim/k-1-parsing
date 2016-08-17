#include "data_types/model/Node.h"
#include <memory>
#include <functional>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#ifndef K1_PARSING_MAPPINGJOB_H
#define K1_PARSING_MAPPINGJOB_H


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


#endif //K1_PARSING_MAPPINGJOB_H
