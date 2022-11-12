# opencv相关

## 1、cvtColor

- `cvtColor`：当图像较大时，直接使用该接口会消耗较多的CPU时间，而且运算时间也较长，可以使用`UMat`的方式，采用`opencl`并行计算提高效率；**采用UMat的opencl方式CPU占用可以减少一半以上；但是内存占用会多一倍；**

  - 实际效果（在UMat没有考虑2次copyTo的开销，只统计cvtColor开销的前提下）

    ![UMat&Mat对比测试结果](./img/cvtColor(UMat-Mat).png)

  - 使用示例

    ```c++
    #include <iostream>
    
    #include "opencv2/opencv.hpp"
    
    void PrintTime(int32_t times, std::string key, int64_t t1, int64_t t0) {
      std::cout << key << "->cvtColor " << times << " times cost: "
        << (t1 - t0) * 1000 / cv::getTickFrequency() << " ms." << std::endl;
    }
    
    void TestMat_cvtColor(int32_t times) {
      cv::Mat src_mat, res_mat;
      src_mat = cv::imread("lean.jpg");
    
      std::cout << "use Mat to test..." << std::endl;
    
      auto t0 = cv::getTickCount();
      for (int32_t i = 0; i < times; ++i) {
        cv::cvtColor(src_mat, res_mat, cv::COLOR_BGR2GRAY);
      }
    
      PrintTime(times, "Mat", cv::getTickCount(), t0);
    }
    
    void TestUMat_cvtColor(int32_t times) {
      cv::Mat src_mat, res_mat;
      src_mat = cv::imread("lean.jpg");
    
      cv::UMat in_umat, out_umat;
    
      std::cout << "use UMat to test..." << std::endl;
    
      src_mat.copyTo(in_umat);
      auto t0 = cv::getTickCount();
      for (int32_t i = 0; i < times; ++i) {
        //src_mat.copyTo(in_umat); // 真实情况应该考虑copyTo开销
        cv::cvtColor(in_umat, out_umat, cv::COLOR_BGR2GRAY);
        //out_umat.copyTo(res_mat); // 真实情况应该考虑copyTo开销
      }
      out_umat.copyTo(res_mat);
    
      PrintTime(times, "UMat", cv::getTickCount(), t0);
    }
    
    // TODO：
    void TestGpuMat_cvtColor(int32_t times) {
      cv::Mat src_mat, res_mat;
	  src_mat = cv::imread("lean.jpg");
    
    cv::cuda::GpuMat in_gpumat, out_gpumat;
    
      std::cout << "use GpuMat to test..." << std::endl;
    
      // TODO：需要opencv编译时支持cuda才行
      auto t0 = cv::getTickCount();
      for (int32_t i = 0; i < times; ++i) {
        in_gpumat.upload(src_mat);
        //cv::cuda::cvtColor(in_gpumat, out_gpumat, cv::COLOR_BGR2GRAY);
        //cv::cvtColor(in_umat, out_umat, cv::COLOR_BGR2GRAY);
        out_gpumat.download(res_mat);
      }
    
      PrintTime(times, "GpuMat", cv::getTickCount(), t0);
    }
    
    int main() {
    
    	int32_t cvt_times = 10000;
    	TestMat_cvtColor(cvt_times);
        TestUMat_cvtColor(cvt_times);
        
        // TODO: wait to do...
        TestGpuMat_cvtColor(cvt_times);
    
    	cv::waitKey();
    	cv::destroyAllWindows();
    
    	return 0;
    }
    ```
    
  - **特殊说明**
  
    按照上面的例子测试会得到上述的结论：**采用UMat更好；但是如果将数据拷贝纳入到统计中时，则可能不会出现使用UMat更好的结论；实际生成环境通常需要加入数据换入换出等环境因素的影响，可以做一个对比测试再来决定使用哪种方案。**
  
  