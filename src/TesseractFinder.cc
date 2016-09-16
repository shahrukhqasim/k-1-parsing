#include "TesseractFinder.h"
using namespace std;
using namespace cv;


TesseractFinder::TesseractFinder()
{	
	this->mode = MODE_FULL;
}


TesseractFinder::TesseractFinder(const string &imagePath) {
    this->imagePath = imagePath;
    this->mode=MODE_FULL;
}

TesseractFinder::TesseractFinder(const string &imagePath, const vector<Rect> &boxes) {
    this->imagePath = imagePath;
    this->boxes=boxes;
    this->mode=MODE_BOXES;
}
void TesseractFinder::run() {

#ifdef TESSERACT_DEBUG_ON
	cout << "Running OCR..." << endl;
#endif
	Pix *image = { pixRead(imagePath.c_str()) }; //the path of image

    recognizeText(image);
	pixDestroy(&image);
}


void TesseractFinder::run(const unsigned char* imagedata, int width, int height,
	int bytes_per_pixel, int bytes_per_line)
{	
	recognizeText(imagedata, width, height, bytes_per_pixel, bytes_per_line);	
}


void TesseractFinder::iterate(tesseract::TessBaseAPI *api) 
{
    tesseract::ResultIterator *ri = api->GetIterator();
    tesseract::PageIteratorLevel level=tesseract::RIL_WORD;	   

    if (ri != 0) 
	{
        do 
		{
            const char *word = ri->GetUTF8Text(level);
			if (word != NULL)
			{	
				OcrResult result;
				result.confidence = ri->Confidence(level);
				ri->BoundingBox(level, &result.word.left, &result.word.top, &result.word.right, &result.word.bottom);

				result.word.text = word;
				delete[] word;

				do
				{	
					auto character = ri->GetUTF8Text(tesseract::RIL_SYMBOL);
					
					if (character != NULL)
					{
						TextAndLocation textAndLocation;
						textAndLocation.text = character;
						ri->BoundingBox(tesseract::RIL_SYMBOL, &textAndLocation.left, &textAndLocation.top, &textAndLocation.right, &textAndLocation.bottom);
						result.characters.push_back(textAndLocation);

						delete[] character;
					}
					if (ri->IsAtFinalElement(tesseract::RIL_WORD, tesseract::RIL_SYMBOL))
					{
						break;
					}

				} while (ri->Next(tesseract::RIL_SYMBOL));

				data.push_back(result);          
            }
        } while (ri->Next(level));
    }
}


void TesseractFinder::recognizeText(const unsigned char* imagedata, int width, int height,
	int bytes_per_pixel, int bytes_per_line)
{

	//RNG rng(12345);
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

	if (!api->SetVariable("textord_no_rejects", "1"))
		printf("Setting variable failed!!!\n");
	
	
	const char *configs[] = { "alphanumeric" };
	int configs_size = 1;

	if (api->Init(NULL, "eng", tesseract::OEM_DEFAULT,const_cast<char**>(configs),configs_size,NULL,NULL,false))
	{
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	
	api->SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
	api->SetImage(imagedata, width, height, bytes_per_pixel, bytes_per_line);

	if (mode == MODE_FULL)
	{
		api->Recognize(0);
		iterate(api);
	}
	else if (mode == MODE_BOXES)
	{
		for (size_t i = 0;i<boxes.size();i++)
		{
			Rect box = boxes[i];
			api->SetRectangle(box.x, box.y, box.width, box.height);
			api->Recognize(0);
			iterate(api);
		}


	}
	else
	{
		cerr << "Tesseract mode not recognized." << endl;
	}


	// Destroy used object and release memory
	api->Clear();
	api->End();

}

void TesseractFinder::recognizeText(Pix* image) {

    //RNG rng(12345);
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (!api->SetVariable("textord_no_rejects", "1"))
        printf("Setting variable failed!!!\n");


	const char *configs[] = { "alphanumeric" };
	int configs_size = 1;


	if (api->Init(NULL, "eng", tesseract::OEM_DEFAULT, const_cast<char**>(configs), configs_size, NULL, NULL, false))   
	{
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    api->SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
    api->SetImage(image);

    if(mode==MODE_FULL) {
        api->Recognize(0);
        iterate(api);
    }
    else if(mode==MODE_BOXES) {
        for(size_t i=0;i<boxes.size();i++) {
            Rect box=boxes[i];
            api->SetRectangle(box.x,box.y,box.width,box.height);
            api->Recognize(0);
            iterate(api);
        }


    }
    else {
        cerr<<"Tesseract mode not recognized."<<endl;
    }


    // Destroy used object and release memory
    api->Clear();
    api->End();
    
}

vector<OcrResult> TesseractFinder::getRecognizedData() {
    return data;
}