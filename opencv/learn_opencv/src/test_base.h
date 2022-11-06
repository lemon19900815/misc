#pragma once

#include "util.h"

namespace testbase {

// 仿射变换
void TestWarpAffine() {
  // 读取原始图像
  cv::Mat image = cv::imread("../images/cat.jpg");

  // 1.尺度变换
  cv::Mat resize_image, resize_image_M;
  cv::resize(image, resize_image, cv::Size(0, 0), 0.5, 0.5, 1);

  cv::Mat warp_matrix_resize; // 声明尺度变换M矩阵
                              // 0 0 0  => 0.75  0     0
                              // 0 0 0  => 0     0.75  0
  warp_matrix_resize = cv::Mat::zeros(2, 3, CV_32F);
  float cx = 0.75, cy = 0.75; // 设置尺度变换M矩阵
  warp_matrix_resize.at<float>(0, 0) = cx;
  warp_matrix_resize.at<float>(1, 1) = cy;

  cv::Size dsize = image.size();
  dsize.width = (int32_t)(dsize.width*cx);
  dsize.height = (int32_t)(dsize.height*cy);
  LOG("M resize = \n" << warp_matrix_resize);

  cv::warpAffine(image, resize_image_M,
    warp_matrix_resize, dsize, cv::INTER_LINEAR, 1);

  //cv::imshow("original", image);
  //cv::imshow("resize", resize_image);
  //cv::imshow("M resize", resize_image_M);

  // 2.平移变换
  cv::Mat temp_image = resize_image.clone(); // 使用缩小后的图片进行变换
  cv::Mat translation_image; // 声明目标图像
  cv::Mat warp_matrix_translation = cv::Mat::eye(2, 3, CV_32F); // 声明平移变换M矩阵
  float tx = 40, ty = 20; // 设置平移变换参数
  warp_matrix_translation.at<float>(0, 2) = tx;
  warp_matrix_translation.at<float>(1, 2) = ty;
  LOG("M translation = \n" << warp_matrix_translation);

  int32_t top = 0, bottom = (int32_t)ty, left = 0, right = (int32_t)tx;
  //cv::copyMakeBorder(temp_image, temp_image,
  //  top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(200));
  // 按照镜像填充
  cv::copyMakeBorder(temp_image, temp_image,
    top, bottom, left, right, cv::BORDER_DEFAULT);
  cv::warpAffine(temp_image, temp_image,
    warp_matrix_translation, temp_image.size(),
    cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);

  cv::imshow("translation", temp_image);

  //3.旋转变换
  cv::Mat rotate_image;
  temp_image = resize_image.clone();
  double angle = 45;
  int32_t border = (int32_t)(0.207 * temp_image.cols);
  // 填充固定值
  cv::copyMakeBorder(temp_image, temp_image,
    border, border, border, border, cv::BORDER_CONSTANT, cv::Scalar(200));
  cv::Point2f center(temp_image.cols / 2.0f, temp_image.rows / 2.0f);
  // 指定旋转中心
  cv::Mat warp_rotate_matrix = cv::getRotationMatrix2D(center, angle, 1.0);
  // 获取旋转变换M矩阵
  // 根据旋转矩阵进行放射变换
  cv::warpAffine(temp_image, rotate_image,
    warp_rotate_matrix, temp_image.size(),
    cv::INTER_LINEAR, cv::BORDER_REPLICATE);
  LOG("M rotate = \n" << warp_rotate_matrix);

  cv::imshow("rotate", rotate_image);

  // 4.剪切变换
  cv::Mat shear_vertical_image, shear_horizontal_image;
  temp_image = resize_image.clone();
  cv::Mat temp_image_vertical, temp_image_horizontal;
  border = 40;
  cv::copyMakeBorder(resize_image, shear_vertical_image,
    10, 2 * border, border, 10 + border, cv::BORDER_CONSTANT, cv::Scalar(200));
  cv::copyMakeBorder(resize_image, shear_horizontal_image,
    border + 10, border, 10, 2 * border, cv::BORDER_CONSTANT, cv::Scalar(200));

  cv::Mat warp_matrix_shear_vertical = cv::Mat::eye(2, 3, CV_32F);
  cv::Mat warp_matrix_shear_horizontal = cv::Mat::eye(2, 3, CV_32F);
  warp_matrix_shear_horizontal.at<float>(0, 1) = 0.3f; // 水平偏移矩阵
  warp_matrix_shear_vertical.at<float>(1, 0) = 0.3f; // 垂直偏移矩阵

  LOG("M shear vertical = \n" << warp_matrix_shear_vertical);
  LOG("M shear horizontal = \n" << warp_matrix_shear_horizontal);

  // 根据旋转矩阵进行仿射变换
  cv::warpAffine(shear_vertical_image, shear_vertical_image,
    warp_matrix_shear_vertical, shear_vertical_image.size(),
    cv::INTER_LINEAR, cv::BORDER_REPLICATE);
  cv::warpAffine(shear_horizontal_image, shear_horizontal_image,
    warp_matrix_shear_horizontal, shear_horizontal_image.size(),
    cv::INTER_LINEAR, cv::BORDER_REPLICATE);
  cv::imshow("shear vertical", shear_vertical_image);
  cv::imshow("shear horizontal", shear_horizontal_image);
}

void DrawHist(std::string win_name, cv::Mat& source) {
  // 需要计算图像的哪个通道（RGB空间需要确定计算R空间、G空间或者B空间）
  const int32_t channels[1] = { 0 };
  // 直方图的bin数目
  int32_t hist_size[] = { 256 };
  // 单个维度直方图数值的取值范围
  float single_ranges[] = { 0, 256 };
  // 确定每个维度的取值范围，即横坐标的总数
  const float* ranges[] = { single_ranges };
  // 输出直方图，采用MatND类型
  cv::MatND dst_hist;
  cv::calcHist(&source, 1, channels, cv::Mat(),
    dst_hist, 1, hist_size, ranges, true, false);

  // 创建一个白底的图像
  cv::Mat draw_image(cv::Size(256, 256), CV_8UC3, cv::Scalar::all(255));
  // 先用cv::minMaxLoc函数计算得到直方图后的像素的最大个数
  double max_value;
  cv::minMaxLoc(dst_hist, 0, &max_value);

  for (int32_t i = 0; i < 256; ++i) {
    // 第i个bin的高度
    auto value = cvRound(256 * 0.9*(dst_hist.at<float>(i) / max_value));
    // 绘制柱状图
    cv::line(draw_image, cv::Point(i, draw_image.rows - 1),
      cv::Point(i, draw_image.rows - 1 - value), cv::Scalar(255));
  }

  cv::imshow(win_name, draw_image);
}

// 直方图均衡化
void TestHist() {
  std::vector<std::string> filenames = {
    "../images/cat.jpg",
    //"../images/cat.jpg",
  };

  cv::Mat image, dst;
  for (int32_t i = 0; i < filenames.size(); ++i) {
    auto id = std::to_string(i);
    image = cv::imread(filenames[i], 0);

    // 直方图均衡化
    cv::equalizeHist(image, dst);
    // 保存均衡化的结果
    auto dst_filename = "../images/dst_" + id + ".jpg";
    cv::imwrite(dst_filename, dst);

    // 计算并显示直方图
    DrawHist("直方图" + id, image);
    DrawHist("直方图均衡化" + id, dst);

    //cv::imshow("image" + id, image);
    //cv::imshow("equalize" + id, dst);
  }
}

void TestLabel() {
  cv::Mat src_img = cv::imread("../images/dogcat.jpg");

  // 1.dog 325*390
  int32_t x = 50, y = 40, width = 300, height = 390;
  cv::Rect rect1(x, y, width, height);
  cv::rectangle(src_img, rect1, cv::Scalar(0, 255, 0), 2);
  cv::putText(src_img, "dog", cv::Point(x, y - 10),
    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2, 3);

  // 2.cat 215*380
  x = 355, y = 40, width = 210, height = 380;
  cv::Rect rect2(x, y, width, height);
  cv::rectangle(src_img, rect2, cv::Scalar(0, 0, 255), 2);
  cv::putText(src_img, "cat", cv::Point(x, y - 10),
    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2, 3);

  // 3.circle
  cv::Point c(src_img.cols / 2, src_img.rows / 2);
  cv::circle(src_img, c, 100, cv::Scalar(255, 0, 0), 1);

  cv::imshow("dot&cat", src_img);
}

void TestVideo() {
  cv::VideoCapture cap;
  if (!cap.open("../images/test.mp4")) {
    LOG("open test.mp4 failed.");
    return;
  }

  cv::Mat frame;
  for (;;) {
    // 读取出来的视频图像是倒立的
    cap >> frame;
    if (frame.empty())
      break;
    // flipCode: >0: 沿y-轴翻转, 0: 沿x-轴翻转, <0: x、y轴同时翻转
    cv::flip(frame, frame, -1);
    cv::imshow("test", frame);
    if (cv::waitKey(33) >= 0)
      break;
  }
}

void Run() {
  TestWarpAffine();
  TestHist();
  TestLabel();
  TestVideo();
}

}
