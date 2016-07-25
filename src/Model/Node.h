//
// Created by Shah Rukh Qasim on 7/25/16.
//

// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <leptonica/allheaders.h>
#include <functional>
#include <memory>

#ifndef K_1_PARSING2_NODE_H
#define K_1_PARSING2_NODE_H

using namespace cv;
using namespace std;

/**
 * This class represents a Document Model node.
 */
class Node {
public:
    /**
     * Rectangular region of the model (x,y,width,height)
     */
    Rect region;

    /**
     * Vector of functions to specify rules e.g. it should be left to specific node or top of it etc
     *
     * @param[in] n must represent this node.
     *
     * The function will return true if the rule is matched otherwise false
     */
    vector<std::function<bool(const Node&n)>> rules;

    /**
     * Represents the subnodes of current node
     */
    vector<shared_ptr<Node>>subNodes;

    /**
     * Only to specify a virtual class
     */
    virtual ~Node();

};


#endif //K_1_PARSING2_NODE_H
