# 加州房价数据
来源是李沐老师在kaggle上发布的一个学习竞赛数据  
可以直接在 https://www.kaggle.com/c/california-house-prices 上下载数据

# 上榜的房价预测代码展示（第二名）  
第二名代码： https://www.kaggle.com/alandata666/automl-l-lightgbm  

目前调整超参数之后只能达到 loss,mae: [0.25754374265670776, 0.25754374265670776]，和kaggle上榜的数据差距较远  
print(model.evaluate(x_test, y_test))

超参数调节后，模型效果不好，需要优化选择特征值；  
第二名的代码中有关于特征数据的重要程度相关的内容，可以参考，作为小白的我，还在学习中
