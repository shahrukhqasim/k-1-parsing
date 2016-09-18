#include <iostream>

#include "stdio.h"
#include <vector>
#include <string>
#include <fstream>
// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <leptonica/allheaders.h>

#include "TesseractFinder.h"
#include "common/EditDistance.h"

#include "../csv/CSVparser.h"
#include "common/HelperMethods.h"
#include "common/Preprocessor.h"

#include "../json/json/json.h"



#ifndef K_1_PARSING2_PROGRAM_H
#define K_1_PARSING2_PROGRAM_H

//struct DocumentModelData {
//	Point p1;
//	Point p2;
//	string exactKeyText;
//};
//
//struct SmallDistance
//{
//	inline bool operator() (const Point& struct1, const Point& struct2)
//	{
//		return (struct1.y-struct1.x > struct2.y-struct2.x);
//	}
//};
//
//struct XFirst
//{
//	inline bool operator() (const Point& struct1, const Point& struct2)
//	{
//		return (struct1.x < struct2.x);
//	}
//};


class OcrProgram {
	/**
	 * A Random Numbers Generator
	 */
    cv::RNG rng= cv::RNG(12345);

	/**
	 * Only file name of the input image
	 */
    std::string inputFileName;

	/**
	 * Path of the input image folder
	 */
    std::string inputFolder;

	/**
	 * Path of the output folder (which will contain JSON files and program output images
	 */
    std::string outputFolder;

	/**
	 * Will contain the cleanedImage (without segmentation
	 */
    cv::Mat cleanedImage;

	/**
	 * Will contain only name of the cleaned file. Cleaned file will be in output folder
	 */
    std::string cleanedImageFileName;

	/**
	 * Result of the OCR job
	 */
    std::vector<OcrResult>data;
//    std::vector<DocumentModelData>documentModelData;

	/**
	 * Matrix of the original untouched image
	 */
    cv::Mat originalImage;

	/**
	 * Loads an image and binarizes it using binarizeShafait
	 *
	 * @param[out] image contains output image matrix
	 * @param[in] fileName contains the file to laod
	 * @param[in] mode shows file loading mode. Which is same as mode
	 * 				imread. 0 for greyscale and 1 for colored image
	 */
    void loadBinaryImage(cv::Mat &image, std::string fileName, int mode);

	//cv::Mat loadImage(std::string fileName, int mode);

	/**
	 * This function does segmentation and stores the result in segments member field
	 */
    
	static void doSegmentation(const cv::Mat& original, std::vector<cv::Rect> &rboxes);

	static cv::Mat RemoveSegments(const cv::Mat& image, std::vector<cv::Rect>& segmentsToRemove);

	/**
	 * This function cleans the image from rulings and store the resultant data to disk
	 * so that tesseract can process it.
	 */
	void cleanImageAndWriteToDisk();

	/**
	 * This function does OCR (using techniques)
	 */
    void runOcr();

	/**
	 * This function outputs the result of OCR to a JSON file specified
	 */
	void outputToJson();

	/**
	 * This function outputs marked OCR results in random colors
	 */
    void outputResult();

	/**
	 * We wrote this function to do a simple one-to-one match for elastic matching but
	 * it is not being employed anywhere at the moment
	 */
//    void oneToOneMatch();


	static std::vector<OcrResult> CleanResults(std::vector<OcrResult>& results);

	static cv::Mat cleanImage(cv::Mat &img);

	/**
	 * This std::vector contains segments of the file using simple segmentation algorithms
	 */
    std::vector<cv::Rect>segments;
public:
    void run();
	
	static std::vector<OcrResult> PerformOCR(std::vector<unsigned char> data);

	/**
	 * Constructor for the OCR job
	 *
	 * @param[in] inputFileName contains only name of the input image file
	 * @param[in] inputFolder contains the complete or relative path of the folder which contains input image
	 * @param[in] outputFolder contains the complete or relative path of the folder on which OCR results will be written
	 */
    OcrProgram(std::string inputFileName, std::string inputFolder, std::string outputFolder);

	/**
	 * This is a static function which runs complete test on the path provided. And also
	 * runs the accuracy test
	 *
	 * @param[in] path is the complete or relative path of the root folder
	 */
    static void runOcrProgram(std::string path);
};


#endif //K_1_PARSING2_PROGRAM_H
