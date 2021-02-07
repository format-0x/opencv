#ifndef OPENCV_IDX_HEADER_H
#define OPENCV_IDX_HEADER_H

#include <iostream>
#include <fstream>

namespace idx {
  class FileHeader {
  private:
    int _magNum;
    int _numItems;
  public:
    [[nodiscard]] unsigned char dataType() const;

    [[nodiscard]] unsigned char numDims() const;

    [[nodiscard]] unsigned numItems() const;

    ~FileHeader();
  };

  class ImageFileHeader : public FileHeader {
  private:
    int _numRows;
    int _numCols;
  public:
    [[nodiscard]] unsigned numRows() const;

    [[nodiscard]] unsigned numCols() const;
  };

  class LabelFileHeader : public FileHeader {};
}

#endif
