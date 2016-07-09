#include "TesseractFinder.h"


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
    recognizeText();
}

void TesseractFinder::iterate(tesseract::TessBaseAPI *api) {
    tesseract::ResultIterator *ri = api->GetIterator();
    tesseract::PageIteratorLevel level=tesseract::RIL_WORD;
//    tesseract::PageIteratorLevel level = iteratorLevel;

    string recognizedText;

    if (ri != 0) {
        do {
            const char *word = ri->GetUTF8Text(level);
            float conf = ri->Confidence(level);
            int x1, y1, x2, y2;

            ri->BoundingBox(level, &x1, &y1, &x2, &y2);


            data.push_back({Point(x1, y1), Point(x2, y2), word==NULL?string():word});

            if(word!=NULL) {
#ifdef TESSERACT_DEBUG_ON
                cout << text << std::endl;
#endif
                recognizedText += word;
                recognizedText += " ";
            }
        } while (ri->Next(level));
    }
}

void TesseractFinder::recognizeText() {

    RNG rng(12345);
#ifdef TESSERACT_DEBUG_ON
    cout << "Running OCR..." << endl;
#endif
    Pix *image = {pixRead(imagePath.c_str())}; //the path of image
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (!api->SetVariable("textord_no_rejects", "1"))
        printf("Setting variable failed!!!\n");

    if (api->Init(NULL, "eng")) {
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
        for(int i=0;i<boxes.size();i++) {
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
    pixDestroy(&image);
}

vector<OcrResult> TesseractFinder::getRecognizedData() {
    return data;
}