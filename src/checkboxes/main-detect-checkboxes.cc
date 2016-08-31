/** 
 * @file pdf-ocr.cc 
 * This is the main file for extracting checkboxes from images.
 * 
 * 
 * @author Faisal Shafait (faisalshafait@gmail.com)  
 * 
 * @version 0.1  
 * 
 */ 

#include "stdio.h" 
#include <string> 
#include "../Preprocessor.h"
#include "checkbox.h"
// Open CV headers 
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
 
using namespace std; 
using namespace cv;


int mainx(int argc, char **argv){
    try {
        if( argc < 2 ){
            fprintf(stderr, "Usage: %s image1 [image2 ... imageN]\n", argv[0]);
            exit(-1);
        }
   //     bool debugMode = false;
        int narg = 1;
        while(narg < argc){
            string inFileName(argv[narg]);
            vector<CCheckBox> cBoxes;
            CDetectCheckBoxes CBDetector;
//            if(debugMode){
//                CBDetector.detectCheckBoxes(inFileName, cBoxes);
//            } else {
                Mat imgMATgray = imread(inFileName, IMREAD_GRAYSCALE); 
                if(imgMATgray.empty()){ 
                    fprintf(stderr, "Error: Could not read image %s with OPENCV! Skipping ...\n", inFileName.c_str()); 
                    return false; 
                }
                Mat imgMAT = imread(inFileName); 
                CBDetector.detectCheckBoxes(imgMATgray, cBoxes);
                // Paint cBoxes on imgMAT
                for(int i=0; i<cBoxes.size(); i++){
                    CCheckBox cb = cBoxes[i];
                    if(!cb.isFilled){
                        if(cb.validOuterBBox){
                            rectangle(imgMAT, cb.outerBBox, Scalar(0,0,255), 2);
                        } else if(cb.validInnerBBox){
                            rectangle(imgMAT, cb.innerBBox, Scalar(0,0,255), 2);
                        }
                    } else {
                        if(cb.validOuterBBox){
                            rectangle(imgMAT, cb.outerBBox, Scalar(255,0,0), 2);
                        } else if(cb.validInnerBBox){
                            rectangle(imgMAT, cb.innerBBox, Scalar(255,0,0), 2);
                        }
                    }
                }
                string::size_type pAtExt = inFileName.find_last_of('.'); 
                string fileBaseName = inFileName.substr(0,pAtExt); 
                string ccFile = fileBaseName + "-cc.png";
                imwrite(ccFile, imgMAT);
          //  }
           
            narg++;   
        }
 
    } catch(const char *s) {
        fprintf(stderr,"FATAL: %s\n",s);
    }
    return 0;
}
 
