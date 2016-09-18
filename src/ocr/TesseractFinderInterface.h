#ifndef TESSERACTFINDERINTERFACE
#define TESSERACTFINDERINTERFACE

#include <string>
#include <vector>


//This is in a separate file so we can include it in a C++/CLI source file.
// C++CLI cannot include the OpenCV headers, so this cannot depend on open CV.


struct TextAndLocation
{
	int left;
	int top;
	int right;
	int bottom;	
	std::string text;
};

/*
* Contains an OCR result entry.
*/
struct OcrResult
{
	float confidence;
	//The whole word
	TextAndLocation word;
	//All of the characters (RIL_SYMOBL) that compose the word
	std::vector<TextAndLocation> characters;
};

#endif



