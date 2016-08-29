
// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "../TextualData.h"



#ifndef K_1_PARSING2_NODE_H
#define K_1_PARSING2_NODE_H


namespace FormParser {
/**
 * This class represents a Document Model node.
 */
class Node {
public:

    static int lastId;

    /**
     * Represents the ID of this node. Can be used to refer from the other nodes.
     */
    std::string id;

    /**
     * Constructs a Node. Will also take care of hierarchy
     */
    Node(bool incrementId=true);

    /**
     * Rectangular region of the model (x,y,width,height)
     */
    cv::Rect region;

    /**
     * Indicates if the region has been defined or not
     */
    bool regionDefined=false;

    /**
     * Vector of functions to specify rules e.g. it should be left to specific node or top of it etc
     *
     * @param[in] n must represent this node.
     *
     * The function will return true if the rule is matched otherwise false
     */
    std::vector<std::function<bool(const TextualData&n)>> rules;

    /**
     * Used to IO of document mode. The key in the super map represents the function
     * name and the std::vector represents what the function applies to.
     *
     * For example, you can set isBelow:3,4,5 which will specify
     * that this node is below nodes with ids 3,4 and 5.
     */
    std::unordered_map<std::string, std::unordered_set<std::string>> rulesModel;

    /**
     * Represents the subnodes of current node
     */
    std::unordered_map<std::string, std::shared_ptr<Node>>subNodes;

    /**
     * Only to specify a virtual class
     */
    virtual ~Node();

};
} // namespace FormParser
#endif //K_1_PARSING2_NODE_H
