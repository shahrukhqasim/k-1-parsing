#include "OcrProgram.h"


#ifndef K_1_PARSING2_WORDENTRY_H
#define K_1_PARSING2_WORDENTRY_H


/**
 * This class represents a text segment defined by its std::string and bounding box
 * */
class TextualData {

private:
    // The bounding box rectangle
    cv::Rect boundingBox;
    // The text
    std::string text;

public:

    /**
     * Will return true if both are nearly equal (alpha-numeric comparison)
     *
     * @param[in] expectedOutput the entry to compare with
     * @return true if matches false otherwise
     */
    bool compare(TextualData &expectedOutput);

    /**
     * Will return true if both entries are exactly same
     *
     * @param[in] expectedOuput the entry to compare with
     * @return true if matches otherwise false
     */
    bool equals(TextualData &expectedOutput);

    /**
     * Get the rectangle of thie entry
     */
    cv::Rect getRect() const;

    /**
     * Get the text of this entry
     *
     * @return the text
     */
    std::string getText() const;

    /**
     * Sets the text of this entry
     *
     * @param[in] input represents the new rectangle
     */
    void setRect(cv::Rect input);

    /**
     * Sets the text of thie entry
     *
     * @param[in] input represents the new text
     */
    void setText(std::string input);

    TextualData operator | (const TextualData& second)const;

    // TODO: Move it to CPP
    bool operator == (const TextualData & d)const {
        return getText()==d.getText()&&getRect()==d.getRect();
    }

};

#endif //K_1_PARSING2_WORDENTRY_H
