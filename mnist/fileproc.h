#ifndef OPENCV_FILEPROC_H
#define OPENCV_FILEPROC_H

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

#include "idx_header.h"

template<class FileHeader>
class FileProc {
  static_assert(std::is_base_of_v<idx::FileHeader, FileHeader>, "FileHeader must derive from idx::FileHeader");
protected:
  FileHeader header;
protected:
  bool read(std::unique_ptr<char[]> &buf, std::size_t size) {
    return ifs->read(buf.get(), size).good();
  };
private:
  std::unique_ptr<std::ifstream> ifs;
private:
  bool readHeader() {
    return ifs->read(reinterpret_cast<char *>(&header), sizeof header).good();
  };
public:
  [[nodiscard]] std::size_t size() const {
    auto fh = dynamic_cast<const idx::FileHeader &>(header);

    return header.numItems();
  };

  void open(const std::string &filename) {
    ifs = std::make_unique<std::ifstream>(filename, std::ios_base::in | std::ios_base::binary);

    if (!ifs->is_open()) {
      throw std::ios_base::failure("No such file: " + filename);
    }

    if (!readHeader()) {
      throw std::ios_base::failure("Can't read the file header");
    }
  };

  FileProc() : ifs{nullptr} {};

  explicit FileProc(const std::string &filename)
      : ifs{std::make_unique<std::ifstream>(filename, std::ios_base::in | std::ios_base::binary)} {

    if (!ifs->is_open()) {
      throw std::ios_base::failure("No such file: " + filename);
    }

    if (!readHeader()) {
      throw std::ios_base::failure("Can't read the file header");
    }
  }
};

class ImageCollection : public FileProc<idx::ImageFileHeader> {
private:
  std::unique_ptr<char[]> buf;
private:
  bool readImage();

public:
  ImageCollection()
      : FileProc<idx::ImageFileHeader>(), buf(std::make_unique<char[]>(header.numRows() * header.numCols())) {};

  explicit ImageCollection(const std::string &filename)
      : FileProc<idx::ImageFileHeader>(filename),
        buf(std::make_unique<char[]>(header.numRows() * header.numCols())) {};

  friend cv::Mat &operator<<(cv::Mat &mat, ImageCollection &ic);
};

class LabelCollection : public FileProc<idx::LabelFileHeader> {
private:
  std::unique_ptr<char[]> buf;
private:
  bool readLabel();

public:
  LabelCollection()
      : FileProc<idx::LabelFileHeader>(), buf(std::make_unique<char[]>(1)) {};

  explicit LabelCollection(const std::string &filename)
      : FileProc<idx::LabelFileHeader>(filename),
        buf(std::make_unique<char[]>(1)) {}

  friend cv::Mat &operator<<(cv::Mat &mat, LabelCollection &lc);
};

#endif
