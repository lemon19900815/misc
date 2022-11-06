#pragma once

#include "util.h"

namespace testkmeans {

void TestKmeans() {
  constexpr int32_t num_clusters = 5;
  cv::Scalar color_tab[] = {
    cv::Scalar(0,0,255),
    cv::Scalar(0,255,0),
    cv::Scalar(255,100,100),
    cv::Scalar(255,0,255),
    cv::Scalar(0,255,255)
  };

  cv::Mat img(500, 500, CV_8UC3); // 创建画布
  img = cv::Scalar::all(255); // 画布设置为白色

  cv::RNG rng(12345);
  for (;;) {
    // 初始化类别数
    int32_t cluster_count = rng.uniform(2, num_clusters + 1);

    // 在指定区间，随机生成一个整数
    // 初始化样本数，随机生成一个整数
    int32_t sample_count = rng.uniform(1, 1001);

    // 输入样本矩阵：sample_count 行x1 列，浮点数，2通道
    cv::Mat points(sample_count, 1, CV_32FC2);

    // 聚类类别数<样本数
    cluster_count = std::min(cluster_count, sample_count);

    // 随机生成多个高斯分布的样本
    for (int32_t k = 0; k < cluster_count; ++k) {
      cv::Point center{ rng.uniform(0, img.cols), rng.uniform(0, img.rows) };

      // 对样本points指定行进行赋值
      cv::Mat pt_chunk = points.rowRange(
        k*sample_count / cluster_count,
        (k + 1)*sample_count / cluster_count);
      //k == cluster_count - 1 ? sample_count : (k + 1)*sample_count / cluster_count);

      // 以center为中心，产生高斯分布的随机点
      rng.fill(pt_chunk, cv::RNG::NORMAL,
        cv::Scalar(center.x, center.y), cv::Scalar(img.cols*0.05, img.rows*.05));
    }

    cv::randShuffle(points, 1.0, &rng);

    // 样本中心点
    std::vector<cv::Point2f> centers;
    // 聚类结果索引矩阵
    cv::Mat labels;

    // 执行k-means：cv::TermCriteria::EPS | cv::TermCriteria::COUNT
    cv::TermCriteria criteria(
      cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0);
    double compactness = cv::kmeans(
      points, cluster_count, labels,
      criteria, 3, cv::KMEANS_PP_CENTERS, centers);

    // 绘制或输出聚类结果
    for (int32_t i = 0; i < sample_count; ++i) {
      auto cluster_idx = labels.at<int32_t>(i);
      auto pt = points.at<cv::Point2f>(i);
      cv::circle(img, pt, 2, color_tab[cluster_idx], cv::FILLED, cv::LINE_AA);
    }

    // 以聚类中心为圆心绘制圆形
    int32_t i = 0;
    for (auto& c : centers) {
      cv::circle(img, c, 40, color_tab[i++], 1, cv::LINE_AA);
    }

    LOG("compactness: " << compactness);
    cv::imshow("clusters", img);
    cv::waitKey();
  }
}

void TestKmeansColor() {
  cv::Mat img = cv::imread("../images/acmilan.jpg");
  LOG("image h = " << img.rows << ", w = "
    << img.cols << ", channels = " << img.channels());

  auto samples = img.reshape(0, img.cols*img.rows);
  // 转换数据类型：CV_32FC3
  samples.convertTo(samples, CV_32FC3);
  LOG("image h = " << samples.rows << ", w = "
    << samples.cols << ", channels = " << samples.channels());

  // 定义kmeans终止条件
  cv::TermCriteria criteria{
    cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 1.0 };
  constexpr int32_t K = 8, flags = cv::KMEANS_PP_CENTERS;
  cv::Mat labels, centers;

  // 执行聚类
  auto compactness = cv::kmeans(
    samples, K, labels, criteria, 3, flags, centers);

  // 按聚类结果重新为图像赋值

  // 将数据转换回CV_8UC3，以便恢复图像
  centers.convertTo(centers, CV_8UC3);
  samples.convertTo(samples, CV_8UC3);

  // 按label标签重新为samples赋值，实现色彩压缩
  std::unordered_map<int32_t, int32_t> cluster_count;
  for (int32_t i = 0; i < labels.rows; ++i) {
    auto cluster = labels.at<int32_t>(i);
    cluster_count[cluster] += 1;
    // Vec3b为opencv中CV_8UC3类型的RGB彩色空间图像数据类型
    samples.at<cv::Vec3b>(i, 0) = centers.at<cv::Vec3b>(cluster, 0);
  }

  // 输出、显示聚类结果

  // 将samples转回img尺寸
  auto img_out = samples.reshape(0, img.rows);

  cv::imshow("image", img);
  cv::imshow("cluster", img_out);

  cv::imwrite("../images/" + std::to_string(K) + "_cluster.jpg", img_out);
}

void Run() {
  TestKmeans();
  TestKmeansColor();
}

}
