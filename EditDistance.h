#ifndef EDIT_DISTANCE_
#define EDIT_DISTANCE_

class EditDistance
{
  public:
    int lDistance (char const *s, char const *t);
  private:
    int minimum (int a, int b, int c);
    int *getCellPointer (int *pOrigin, int col, int row, int nCols);
    int getAt (int *pOrigin, int col, int row, int nCols);
    void putAt (int *pOrigin, int col, int row, int nCols, int x);
};

#endif
