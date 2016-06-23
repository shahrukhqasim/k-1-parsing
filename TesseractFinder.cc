#include "TesseractFinder.h"


TesseractFinder::TesseractFinder(const string&imagePath) {
  this->imagePath=imagePath;
}

void TesseractFinder::run() {
  recognizeText();
}

void TesseractFinder::recognizeText() {

	RNG rng(12345);
  cout<<"Running OCR..."<<endl;
    Pix *image = { pixRead(imagePath.c_str()) }; //the path of image
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (api->Init(NULL, "eng")) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      exit(1);
    }

    api->SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
    api->SetImage(image);
    api->Recognize(0);

    tesseract::ResultIterator* ri = api->GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_TEXTLINE;

    string recognizedText;

    if (ri != 0) {
      do {
        const char* word = ri->GetUTF8Text(level);
        float conf = ri->Confidence(level);
        int x1, y1, x2, y2;

        ri->BoundingBox(level, &x1, &y1, &x2, &y2);


        data.push_back({Point(x1,y1),Point(x2,y2),word});

        cout << word << std::endl;
        recognizedText+=word;
        recognizedText+=" ";
      } while (ri->Next(level));
    }

    // Destroy used object and release memory
    api->Clear();
    api->End();
    pixDestroy(&image);
}

vector<KeyData> TesseractFinder::getRecognizedData() {
  return data;
}
