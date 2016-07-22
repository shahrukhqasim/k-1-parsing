//
// Created by Talha Rehman on 6/24/16.
//
#include "OcrProgram.h"

#ifndef K_1_PARSING2_WORDENTRY_H
#define K_1_PARSING2_WORDENTRY_H


/**
 * This class represents a text segment defined by its string and bounding box
 * */
class TextualData {

private:
    // The bounding box rectangle
    Rect boundingBox;
    // The text
    string text;

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
    Rect getRect() const;

    /**
     * Get the text of this entry
     *
     * @return the text
     */
    string getText() const;

    /**
     * Sets the text of this entry
     *
     * @param[in] input represents the new rectangle
     */
    void setRect(Rect input);

    /**
     * Sets the text of thie entry
     *
     * @param[in] input represents the new text
     */
    void setText(string input);

    TextualData operator | (const TextualData& second)const;

};

#endif //K_1_PARSING2_WORDENTRY_H
