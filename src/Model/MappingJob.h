//
// Created by shahrukhqasim on 8/5/16.
//

#include "Node.h"

#ifndef K_1_PARSING2_MAPPINGJOB_H
#define K_1_PARSING2_MAPPINGJOB_H


class MappingJob {
    shared_ptr<Node>ultimateParent;
    Rect lastRect;
    string nodeId;
    int width;
    int height;
public:
    MappingJob(const shared_ptr<Node> &ultimateParent, const string &nodeId, int width, int height);

    Rect map();
private:
    void recursive(shared_ptr<Node> node);
};


#endif //K_1_PARSING2_MAPPINGJOB_H
