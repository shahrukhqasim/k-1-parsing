#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include "fstream"

#ifndef HELPERMETHODS_H_
#define HELPERMETHODS_H_



/**
 * This is a static class which only contains different helper methods
 */
class HelperMethods {
public:

    /**
     * Displays image in @image in a resizable window with the help of programs installed on the host machine
     * */
    static void displayImageResizable(const cv::Mat &image);


    /**
     * Outputs image to a file
     *
     * @param[in] image represents the output image
     * @param[in] fileName represents the output file path
     *
     * */
    static void outputImage(const cv::Mat &image, const std::string &fileName);

    /**
     * Removes file extension from its path or file name
     *
     * @param[in] fileName represents the file path
     *
     * @return the file name or path without extension text
     *
     * */
    static std::string removeFileExtension(const std::string &fileName);

    /**
     * To check if the character is an alpha numeric character (and not a space)
     *
     * @param[in] a is the character to be checked
     *
     * @return true if it is an alpha numeric; false otherwise
     *
     * */
    static bool isAlphaNumericNotSpace(char a);

    /**
     * To do a near equal (alpha-numeric) comparison
     *
     * @param[in] a represents first std::string
     * @param[in] b represents second std::string
     *
     * @return true if both are nearly equal; false otherwise
     *
     * */
    static bool nearEqualComparison(const std::string&a,const std::string&b);

    /**
     * Splits a std::string to parts at regular expression provided
     *
     * @param[in] s represents the input std::string
     * @param[in] rgx_str represents the regular expression. Default is whitespace
     *
     * */

    static void convertPdfs();

    static std::vector<std::string> regexSplit(const std::string & s, std::string rgx_str = "\\s+");
};

#endif /* HELPERMETHODS_H_ */
