#pragma once

#include "util.h"

namespace testdtree {

void MushroomClassify() {
  std::string csv_filename = "../datasets/mushroom/agaricus-lepiota.data";

  // 1.读取csv文件
  auto dataset = cv::ml::TrainData::loadFromCSV(
    csv_filename, 0, 0, 1, "cat[0-22]");

  // 2.验证数据读取是否正确
  auto n_samples = dataset->getNSamples();
  if (n_samples == 0) {
    LOG("ERROR: read " << csv_filename << " failed.");
    return;
  }

  LOG("sample count = " << n_samples);

  // 3.划分训练集与测试集
  dataset->setTrainTestSplitRatio(0.7, true);

  auto n_train = dataset->getNTrainSamples();
  auto n_test = dataset->getNTestSamples();

  LOG("n_train = " << n_train << ", n_test = " << n_test);

  // 4.创建决策树
  auto dtree = cv::ml::RTrees::create();

  // 5.设置模型参数
  // 先验类概率数组：有毒和无毒的错误权重，有毒的预测成无毒的危害更大，所以权值需要偏移；
  cv::Mat priors(1, 2, CV_32F, { 1.0, 100.0 });
  // 决策树的最大深度
  constexpr int32_t kDTreeMaxDepth = 8;
  // 样本最小分裂子节点的数量，低于该值，不会在划分子节点
  constexpr int32_t kMinSampleCount = 10;
  // 回归树的终止条件，用于分类的决策树不设置
  constexpr float kRegressionAccuracy = 0.01f;
  // 是否构建代理切分标志：如果确实某些特征值时，该样本无法继续向下切分，
  // 也就无法达到叶子节点，使用代理切分则利用当前节点下的所有子节点来预测输出的平均值，
  // 作为这个样本的预测输出；如果需要计算特征重要性，则需要将代理区分标志位设置为true
  // opencv算法没有实现该属性，如果设置为true，会抛出异常
  constexpr bool kUseSurrogates = false;
  // 蘑菇分类的类别数
  constexpr int32_t kMaxCategroies = 15;
  // k折交叉验证
  constexpr int32_t kCVFolds = 1;
  // 如果设置为true，剪枝将应用1SE规则，这将使树更紧凑，
  // 更能抵抗训练数据噪声，但准确度稍差，默认值为true。
  constexpr bool kUse1SERule = true;
  // 如果标志位为true，则从树中物理移除已修剪的分支，否则它们会被保留，
  // 并且可以从原始未修剪（或修剪得不那么大）的树中获得结果。默认值为true
  constexpr bool kTruncatePrunedTree = true;

  dtree->setMaxDepth(kDTreeMaxDepth);
  dtree->setMinSampleCount(kMinSampleCount);
  //dtree->setRegressionAccuracy(kRegressionAccuracy);
  dtree->setUseSurrogates(kUseSurrogates);
  dtree->setMaxCategories(kMaxCategroies);
  dtree->setCVFolds(kCVFolds);
  dtree->setUse1SERule(kUse1SERule);
  dtree->setTruncatePrunedTree(kTruncatePrunedTree);
  dtree->setPriors(priors);

  // 6.训练决策树模型
  LOG("start train...");
  dtree->train(dataset);

  LOG("finish train...");

  // 7.测试
  cv::Mat train_results, test_results;
  auto train_error = dtree->calcError(dataset, false, train_results);
  auto test_error = dtree->calcError(dataset, true, test_results);

  std::vector<cv::String> names;
  dataset->getNames(names);

  auto flags = dataset->getVarSymbolFlags();

  // 8.统计输出结果
  auto expected_res = dataset->getTestResponses();

  int32_t right = 0, wrong = 0;
  for (int32_t i = 0; i < n_test; ++i) {
    auto res = (int32_t)test_results.at<float>(i, 0);
    auto expected = (int32_t)expected_res.at<float>(i, 0);

    auto res_str = names[res];
    auto excepted_str = names[expected];

    if (res == expected) {
      ++right;
      //LOG("label: " << excepted_str << ", predict: " << res_str);
    } else {
      ++wrong;
      //LOG("label: " << excepted_str << ", predict: " << res_str << " x.");
    }
  }

  LOG("right = " << right << ", wrong = "
    << wrong << ", total test samples = " << n_test);
  LOG("[Decision Tree] " << std::setprecision(4)
    << "right result = " << (float)right / n_test
    << ", wrong result = " << (float)wrong / n_test);
  LOG("[Decision Tree]: " << std::setprecision(4)
    << "train error: " << train_error << ", test error: " << test_error);
  //LOG("[Decision Tree]: " << std::setprecision(4)
  //  << "test acc: " << (float)right / n_test * 100
  //  << "%, test error: " << (float)wrong / n_test * 100 << "%");

  dtree->save("./dtree_01.xml");

  auto samples = dataset->getTrainSamples();
  auto sample = samples.row(0);

  // 数据转录：从原始csv的数据集；由于接口没有返回nameMap在自我转录时，需要构建映射表
  std::map<cv::String, int32_t> nameMap;
  int32_t i = 0;
  for (auto& name : names) {
    nameMap[name] = i++;
  }

  // 分类类别的数据；如果本身数据就是numberic数字，则不需要转换
  auto getVarByName = [&nameMap](cv::String s) ->int32_t {
    auto it = nameMap.find(s);
    return it != nameMap.end() ? it->second : -1;
  };

  // csv中的数据，源码中使用decodeEle转录数据
  std::vector<char> orignal_data{
    'x','s','n','t','p','f','c','n','k','e',
    'e','s','s','w','w','p','w','o','p','k','s','u' };
  std::vector<float> categorical_data;

  for (auto& ch: orignal_data) {
    categorical_data.push_back(getVarByName(cv::String(1, ch)));
  }
  cv::Mat tsamples(1, categorical_data.size(), CV_32F, categorical_data.data());//&categorical_data[0]);

  std::stringstream ss;
  for (int32_t i = 0; i < sample.cols; ++i) {
    ss << static_cast<int>(sample.at<float>(0, i)) << ", ";
  }

  LOG("sample: " << ss.str());

  std::stringstream temp;
  for (int32_t i = 0; i < tsamples.cols; ++i) {
    temp << static_cast<int>(tsamples.at<float>(0, i)) << ", ";
  }
  LOG("temp sample: " << temp.str());

  auto result = dtree->predict(sample);
  auto result2 = dtree->predict(tsamples);

  LOG("res = " << result);
}

void Run() {
  // sample 1. mushroom classify.
  MushroomClassify();

  // sample 2. Boston Housing
}

}
