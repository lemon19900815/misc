#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include <cstdint>
#include <vector>

#include <opencv2/opencv.hpp>

#define LOG(str) std::cout << __FUNCTION__ << ":" << __LINE__ << ": " << str << std::endl;
#define LOG_IF(cond, str) if(cond) LOG(str)

// void Plot(std::string name, std::vector<cv::Point2f>& pts) {
  // // 创建画布
  // cv::Mat canvas(800, 600, CV_32FC1);
  // canvas = cv::Scalar::all(255);

  // for (int32_t i = 0; i+1 < pts.size(); ++i) {
    // cv::line(canvas, pts[i], pts[i + 1], cv::Scalar(0, 0, 255));
  // }

  // cv::imshow(name, canvas);
// }
