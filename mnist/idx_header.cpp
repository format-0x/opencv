#include "idx_header.h"

idx::FileHeader::~FileHeader() {
  std::cout << "~FileHeader()\n";
}

unsigned idx::ImageFileHeader::numCols() const {
  return __builtin_bswap32(_numCols);
}

unsigned idx::ImageFileHeader::numRows() const {
  return __builtin_bswap32(_numRows);
}

unsigned idx::FileHeader::numItems() const {
  return __builtin_bswap32(_numItems);
}

unsigned char idx::FileHeader::dataType() const {
  return (_magNum >> 16) & 0xFF;
}

unsigned char idx::FileHeader::numDims() const {
  return (_magNum >> 24) & 0xFF;
}
