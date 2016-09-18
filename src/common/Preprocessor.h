#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include "common/UnionFind.h"


#ifndef K_1_PARSING2_PREPROCESSOR_H
#define K_1_PARSING2_PREPROCESSOR_H


class Preprocessor {
public:
    /**
     * This function uses union find structures to build connected components
     * in a horizontal and a vertical sweep through the image
     *
     * @param[in]     img Input CV_8U Mat data structure representing a binary image
     *                Foreground is assumed 0 and background is 255.
     * @param[in]     max_x Maximum allowed width of a connected component as
     *                a fraction of image width
     * @param[in]     max_y Maximum allowed height of a connected component as
     *                a fraction of image height
     * @param[in]     min_area Minimum allowed area (width x height) of a valid
     *                connected component.
     * @param[in]     type Use 4 or 8 connected neighborhood.
     * @param[out]    rboxes An array of bounding boxes of valid connected components.
     * @return        Pointer to the newly created Pix data structure.
     *
     */
    static void conCompFast(const cv::Mat &img, std::vector<cv::Rect> &rboxes, float max_x = 1.0, float max_y = 1.0,
                            float min_area = 0, int type = 8);


    /**
     * This function implements local adaptive thresholding algorithms from:
    * Faisal Shafait, Daniel Keysers, Thomas M. Breuel. "Efficient Implementation
    * of Local Adaptive Thresholding Techniques Using Integral Images",
    * SPIE Document Recognition and Retrieval XV, DRR'08, San Jose, CA, USA. Jan. 2008
    *
    * @param[in]     gray Gray scale input image as an OpenCV Mat object.
     * @param[out]    binary Output image binarized using Shafait's method.
    * @param[in]     w local square window side length to compute adaptive threshold.
    * @param[in]     k Gray level sensititivity parameter. Lower values of k result in whiter images (fewer black pixels) and vice versa.
    */
    static void binarizeShafait(cv::Mat &gray, cv::Mat &binary, int w, double k);

    /**
     * Inverts colors of 8-bit @image
     */
    static void invertImage(cv::Mat &image);
};


#endif //K_1_PARSING2_PREPROCESSOR_H
