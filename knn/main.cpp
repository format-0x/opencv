#include <iostream>
#include <exception>

#include "fileproc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "\"K\" value is not provided\n";

    return EXIT_FAILURE;
  }

  std::istringstream iss(argv[1]);

  int k;

  if (!(iss >> k)) {
    std::cout << "Incorrect value for \"K\"\n";

    return EXIT_FAILURE;
  }

  ImageCollection trainImagesCol;
  LabelCollection trainLabelsCol;
  ImageCollection testImagesCol;
  LabelCollection testLabelsCol;

  try {
    trainImagesCol.open("data/train-images-idx3-ubyte");
    trainLabelsCol.open("data/train-labels-idx1-ubyte");
    testImagesCol.open("data/t10k-images-idx3-ubyte");
    testLabelsCol.open("data/t10k-labels-idx1-ubyte");
  } catch (const std::ios_base::failure &exception) {
    std::cout << exception.what() << std::endl;

    return EXIT_FAILURE;
  }

  cv::Mat trainImages;
  cv::Mat trainLabels;
  cv::Mat testImages;
  cv::Mat testLabels;

  double t = cv::getTickCount();

  trainImages << trainImagesCol;
  trainLabels << trainLabelsCol;
  testImages << testImagesCol;
  testLabels << testLabelsCol;

  t = (cv::getTickCount() - t) / cv::getTickFrequency();

  std::cout << "Data read in " << t << "s\n";

  cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();

  double t1 = cv::getTickCount();

  knn->train(trainImages, cv::ml::ROW_SAMPLE, trainLabels);

  t1 = (cv::getTickCount() - t1) / cv::getTickFrequency();

  std::cout << "Model trained in " << t1 << "s\n";

  int errors = 0;

  double t2 = cv::getTickCount();

  for (int i = 0; i < testImages.rows; ++i) {
    cv::Mat prediction;
    cv::Mat neighbours;

    knn->findNearest(testImages.row(i), k, prediction, neighbours);

    int p = static_cast<int>(prediction.at<float>(0));
    int l = testLabels.at<int>(i);

    if (p != l) {
      errors += 1;

      std::cerr << l << " : " << p << " " << neighbours << std::endl;
    }
  }

  t2 = (cv::getTickCount() - t2) / cv::getTickFrequency();

  std::cout << "Classification took: " << t2 << "s\n";
  std::cout << "Error rate(%): " << errors * 100 / testImages.rows << std::endl;

  return EXIT_SUCCESS;
}