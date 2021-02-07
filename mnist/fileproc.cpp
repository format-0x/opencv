#include "fileproc.h"

bool ImageCollection::readImage() {
  return read(buf, header.numRows() * header.numCols());
}

bool LabelCollection::readLabel() {
  return read(buf, 1);
}

cv::Mat &operator<<(cv::Mat &mat, ImageCollection &ic) {
  cv::Mat outMat;

  while (ic.readImage()) {
    cv::Mat(ic.header.numRows(), ic.header.numCols(), CV_8U, &ic.buf[0])
        .reshape(0, 1)
        .clone()
        .convertTo(outMat, CV_32F);

    mat.push_back(outMat);
  }

  return mat;
}

cv::Mat &operator<<(cv::Mat &mat, LabelCollection &lc) {
  while (lc.readLabel()) {
     mat.push_back(cv::saturate_cast<int>(lc.buf[0]));
  }

  return mat;
}
