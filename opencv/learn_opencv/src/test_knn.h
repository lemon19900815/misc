#pragma once

#include "util.h"

namespace testknn {

// 制作数据集
void GenerateDataset(
    cv::Mat& img, cv::Mat& X_train, cv::Mat& X_test,
    cv::Mat& y_train, cv::Mat& y_test, int32_t train_rows) {

  constexpr int32_t width_slice = 20, height_slice = 20;
  constexpr int32_t n_row_sample = 100, n_col_sample = 50;

  constexpr int32_t row_single_num = 5; // 单类样本占5行
  int32_t test_rows = row_single_num - train_rows;

  constexpr int32_t classify_num = 10; // 数字分类类别数为10

  //cv::Mat train(train_rows * 20 * classify_num, img.cols, CV_8UC1);
  //train = cv::Scalar::all(0);

  //cv::Mat test(test_rows * 20 * classify_num, img.cols, CV_8UC1);
  //test = cv::Scalar::all(0);

  LOG("start generate train data and test data...");

  cv::Rect roi;
  cv::Mat single_num_img;
  for (int32_t col = 0; col < n_col_sample; ++col) {
    for (int32_t row = 0; row < n_row_sample; ++row) {
      roi = cv::Rect(row*width_slice, col*height_slice,
        width_slice, height_slice);
      single_num_img = img(roi).clone();

      auto& cur_data = (col + 1) % 5 < train_rows ? X_train : X_test;
      cur_data.push_back(single_num_img.reshape(0, 1));
    }
  }

  X_train.convertTo(X_train, CV_32FC1);
  X_test.convertTo(X_test, CV_32FC1);

  // 标签生成器
  auto generate_label = [n_row_sample](
      int32_t label, int32_t num_rows, cv::Mat& y) {
    auto num_rows_sample = num_rows * n_row_sample;
    cv::Mat mat_label = cv::Mat::ones(num_rows_sample, 1, CV_32FC1);
    mat_label = mat_label * label;

    auto y_chunk = y.rowRange(
      label * num_rows_sample, (label + 1) * num_rows_sample);
    mat_label.copyTo(y_chunk);
  };

  // 生成标签
  for (int32_t i = 0; i < classify_num; ++i) {
    // train labels.
    generate_label(i, train_rows, y_train);
    // test labels.
    generate_label(i, test_rows, y_test);
  }

  LOG("label data is generated.");
}

void Run() {
  auto img = cv::imread("../images/digits.png", cv::IMREAD_GRAYSCALE);
  //cv::cvtColor(img, img, CV_BGR2GRAY);

  // digits每行100个数字，每列50个数字
  constexpr int32_t n_train = 4000, n_test = 1000;
  constexpr int32_t train_rows = 4; // 前4行为训练样本，第5行为测试样本

  cv::Mat X_train, X_test;
  cv::Mat y_train(n_train, 1, CV_32FC1);
  cv::Mat y_test(n_test, 1, CV_32FC1);

  // 生成训练集、测试集与标签数据
  GenerateDataset(img, X_train, X_test, y_train, y_test, train_rows);

  bool silent = true;
  float best_acc = 0.0f;
  int32_t best_K = 0;
  std::vector<cv::Point2f> pts;

  for (int32_t K = 1; K <= 30; ++K) {
    // 创建KNN模型
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();

    // 考察最近邻样本个数
    //constexpr int32_t K = 2;
    knn->setDefaultK(K);
    knn->setIsClassifier(true);
    knn->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);

    // 训练模型
    LOG_IF(!silent, "train knn model...");
    knn->train(X_train, cv::ml::ROW_SAMPLE, y_train);

    LOG_IF(!silent, "preparing for test...");

    cv::Mat res;
    knn->findNearest(X_test, K, res);

    int32_t n_error = 0;
    for (int32_t i = 0; i < n_test; ++i) {
      auto pred = res.at<float>(i);
      auto label = y_test.at<float>(i);

      if (pred != label) {
        n_error += 1;
        LOG_IF(!silent, "ERROR: label: " << label << ", pred: " << pred);
      }
    }

    auto acc = (n_test - n_error)*1.0f / n_test;
    if (acc > best_acc) {
      best_acc = acc;
      best_K = K;
    }

    LOG("K = " << K << ", accuracy = " << acc);
    pts.emplace_back((float)K, acc);
  }

  LOG("best K = " << best_K << ", best acc = " << best_acc);

  //Plot("knn", pts);
}

}

