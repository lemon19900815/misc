#coding=utf8

import os
import sys

import tensorflow as tf
import pandas as pd
import tensorflow.keras.backend as K
from tensorflow.keras import models,layers,losses,metrics,callbacks, optimizers
import matplotlib.pyplot as plt
import numpy as np
import math

import tools

ds_train_raw = pd.read_csv('train.csv')
ds_test_raw = pd.read_csv('test.csv')

# 验证相关性
def corrCol(dfdata, x, y):
    dfdata.plot.scatter(x=x, y=y)
    plt.show()

#corrCol(ds_train_raw, 'Listed Price', 'Sold Price')
#corrCol(ds_train_raw, 'Zip', 'Sold Price')

# 正则化
def norm(data):
    #return (data - data.mean())/data.std()
    return data

# 填充缺失值
def fillMedian(data):
    data.iloc[data<=0] = data.median()
    return data

# 拆分数据集
def splitData(data, split_rate):
    assert(split_rate>=0 and split_rate<=1)
    idx = math.ceil(data.shape[0]*(1-split_rate))
    return data[0:idx],data[idx:]

# 数据预处理
def preprocessing(dfdata):
    dfresult = pd.DataFrame()

    # SingleFamily:1,Condo:2,Other:0
    #dfresult['Type'] = dfdata['Type'].apply(lambda x: 1 if x=='SingleFamily' else (2 if x=='Condo' else 0))

    dfresult['ListedPrice'] = np.log(fillMedian(dfdata['Listed Price']))
    dfresult['YearBuilt'] = dfdata['Year built'].fillna(0)
    dfresult['FullBathrooms'] = dfdata['Full bathrooms'].fillna(0)
    dfresult['ElementarySchoolScore'] = dfdata['Elementary School Score'].fillna(0)
    dfresult['ElementarySchoolDistance'] = dfdata['Elementary School Distance'].fillna(0)
    dfresult['HighSchoolScore'] = dfdata['High School Score'].fillna(0)
    dfresult['HighSchoolDistance'] = dfdata['High School Distance'].fillna(0)
    #dfresult['TaxValue'] = norm(dfdata['Tax assessed value'].fillna(0))
    #dfresult['AnnualTaxAmount'] = norm(dfdata['Annual tax amount'].fillna(0))

    return dfresult

x_train = preprocessing(ds_train_raw)
y_train = np.log(fillMedian(ds_train_raw['Sold Price']))

x_train, x_test = splitData(x_train, 0.2)
y_train, y_test = splitData(y_train, 0.2)

print('x_train.shape =', x_train.shape)
print('y_train.shape =', y_train.shape)

print('x_test.shape =', x_test.shape)
print('y_test.shape =', y_test.shape)

class RMSE(losses.Loss):
    def call(self, y_true, y_pred):
        y_true = K.cast(y_true, y_pred.dtype)
        return K.sqrt(K.mean(K.square(y_pred - y_true), axis=-1))

    def get_config(self):
        config = super(RMSE, self).get_config()
        return config

input_shape = (len(x_train.columns),)

# units从第一层网络到最后一层网络
def buildModel(lr, units=[20,10]):
    K.clear_session()

    model = tf.keras.Sequential()

    model.add(layers.Dense(units[0], activation='relu', input_shape=input_shape))
    for i, unit in enumerate(units, start=1):
        model.add(layers.Dense(unit, activation='relu'))
    model.add(layers.Dense(1))

    # 显示训练模型信息
    # model.summary()

    model.compile(
        optimizer=optimizers.Adam(learning_rate=lr),
        #loss='mse',
        loss=RMSE(name='RMSE'),
        metrics=['mae'])

    return model

def trainModel(lr, batch_size=32, units=[20, 10]):
    model = buildModel(lr, units)
    history = model.fit(x=x_train,
        y=y_train,
        validation_split=0.2,
        batch_size=batch_size,
        epochs=10,
        verbose=0)

    return history

def optimize(name, params, lr=0.05, batch_size=32, units=[20,10]):
    min_loss_list = []
    min_mae_list = []

    for param in params:
        if name == 'lr':
            lr = param
        elif name == 'batch_size':
            batch_size = param
        elif name == 'units':
            units = param
        else:
            return None
        hist = trainModel(lr, batch_size, units)
        min_loss_list.append(np.nanmin(hist.history['loss']))
        min_mae_list.append(np.nanmin(hist.history['mae']))

    loss_idx = np.nanargmin(min_loss_list)
    mae_idx = np.nanargmin(min_mae_list)

    print(name, ':', params)
    print('history loss list =', min_loss_list)
    print('history mae list =', min_mae_list)

    print('loss optimize %s ='%name, params[loss_idx], ', loss =', min_loss_list[loss_idx])
    print('mae optimize %s ='%name, params[mae_idx], ', mae =', min_mae_list[mae_idx])

    return params[loss_idx]

# 优化选择lr
def optimizeLearningRate(lr_list, batch_size=32, units=[20,10]):
    return optimize('lr', lr_list, 0.01, batch_size, units)

# 优化选择batch_size
def optimizeBatchSize(batch_size_list, lr=0.05, units=[20,10]):
    return optimize('batch_size', batch_size_list, lr, 32, units)

# 优化选择units
def optimizeUnits(units_list, lr=0.05, batch_size=32):
    return optimize('units', units_list, lr, batch_size)

# 超参数优化选择
# 1、首先调整学习率alpha
# 2、调整batch_size
# 3、调整模型
# 4、精细化调整学习率alpha
lr = optimizeLearningRate([0.01, 0.001, 0.05, 0.005, 0.1])
batch_size = optimizeBatchSize([32, 64, 128], lr)
units = optimizeUnits([[20,10], [32,16,8], [20,10,5], [16,8]], lr, batch_size)

# lr = 0.01
# batch_size = 32
# units = [20,10]

# 优化后的超参数
print('lr =', lr, 'batch_size =', batch_size, 'units =', units)

model = buildModel(lr, units)
model.summary()

history = model.fit(
        x=x_train,
        y=y_train,
        validation_split=0.2,
        batch_size=batch_size,
        epochs=10)

tools.plot_history(history, ['loss','mae'])

# [0.25754374265670776, 0.25754374265670776]
print(model.evaluate(x_test, y_test))

print(np.exp(y_test.head()))
print(np.exp(model.predict(x_test.head())))
