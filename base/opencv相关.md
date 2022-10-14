# opencv相关

## 1、cvtColor

- `cvtColor`：当图像较大时，直接使用该接口会消耗较多的CPU时间，而且运算时间也较长，可以使用`UMat`的方式，采用`opencl`并行计算提高效率；**采用UMat的opencl方式CPU占用可以减少一半以上；速度提升10倍+**

  - 实际效果

    ![UMat&Mat对比测试结果](./img/cvtColor(UMat-Mat).png)

  - 使用示例

    ```c++
    #include <iostream>
    
    #include "opencv2/opencv.hpp"
    
    int main() {
    
    	cv::Mat src_mat, res_mat;
    	src_mat = cv::imread("lean.jpg");
    
    	int64_t t0 = 0, t1 = 0;
    	int32_t cvt_times = 10000;
    
    	cv::UMat in_umat, out_umat;
    	src_mat.copyTo(in_umat);
    
    	auto print_time = [cvt_times](std::string key, int64_t t1, int64_t t0) {
    		std::cout << key << "->cvtColor " << cvt_times << " times cost: "
    			<< (t1 - t0) * 1000 / cv::getTickFrequency() << " ms." << std::endl;
    	};
    
    	t0 = cv::getTickCount();
    	for (int32_t i = 0; i < cvt_times; ++i) {
    		cv::cvtColor(in_umat, out_umat, cv::COLOR_BGR2GRAY);
    	}
    	t1 = cv::getTickCount();
    	print_time("UMat", t1, t0);
    	out_umat.copyTo(res_mat);
    
    	t0 = cv::getTickCount();
    	for (int32_t i = 0; i < cvt_times; ++i) {
    		cv::cvtColor(src_mat, res_mat, cv::COLOR_BGR2GRAY);
    	}
    	t1 = cv::getTickCount();
    	print_time("Mat", t1, t0);
    
    	//outUMat.copyTo(outMat);
        // 不要使用这种方式去获取转换后的Mat，否则引用计数清除不掉的问题
    	//outMat = outUMat.getMat(cv::ACCESS_READ);
    
    	cv::imshow("lean", res_mat);
    	cv::waitKey();
    	cv::destroyAllWindows();
    
    	return 0;
    }
    ```

    

  