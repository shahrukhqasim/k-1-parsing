#include "TextualData.h"
using namespace std;
using namespace cv;



bool TextualData::compare(TextualData &expectedOutput){

    int matched=0;

    // Jc = (Box1 intersection Box2)/(Box1 union Box2)
    float Jc =float((this->boundingBox & expectedOutput.getRect()).area())/( this->boundingBox | expectedOutput.getRect() ).area() ;


    // If an intersection occurs, do a near equal comparison (which only matches alpha numeric text)
    if(Jc>0.1){
        if(HelperMethods::nearEqualComparison(this->text , expectedOutput.getText()) )
            return 1;
    }

    return 0;

}

bool TextualData::equals(TextualData &expectedOutput) {
    if(this->getRect()==expectedOutput.getRect()&& this->getText()== expectedOutput.getText())
        return true;
    else
        return false;
}

Rect TextualData::getRect() const{
    return this->boundingBox;
}

string TextualData::getText() const{
    return this->text;
}

void TextualData::setRect(Rect input){
    boundingBox=input;
}

void TextualData::setText(string input){
    text=input;
}

TextualData TextualData::operator|(const TextualData &second) const {
    TextualData d;
    d.setText(getText()+" "+second.getText());
    d.setRect(getRect()|second.getRect());
    return d;
}