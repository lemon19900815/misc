#coding=utf8

import tensorflow as tf
from matplotlib import pyplot as plt

def plot_history(history, metrics):
    plt.figure(figsize=(12,5))
    for i,metric in enumerate(metrics):
        plot_metric(history, metric, plt.subplot(1, len(metrics), i+1))
    plt.show()

def plot_metric(history, metric, ax):
    train_metrics = history.history[metric]
    val_metrics = history.history['val_'+metric]
    epochs = range(1, len(train_metrics)+1)

    # 如果不是子图像就创建新的图像并绘制显示
    if not ax:
        plt.figure()

    plt.plot(epochs, train_metrics, 'bo--')
    plt.plot(epochs, val_metrics, 'ro--')
    plt.title('Training and validation ' + metric)
    plt.xlabel('Epochs')
    plt.ylabel(metric)
    plt.legend(['train_'+metric, 'val_'+metric])

    if not ax:
        plt.show()

#打印时间分割线
@tf.function
def printbar():
    today_ts = tf.timestamp() % (24*60*60)

    hour = tf.cast((today_ts/3600+8) % 24, tf.int32)
    minute = tf.cast(today_ts % 3600/60, tf.int32)
    second = tf.cast(tf.floor(today_ts % 60), tf.int32)

    def timeformat(m):
        if tf.strings.length(tf.strings.format('{}', m)) == 1:
            return tf.strings.format('0{}', m)
        else:
            return tf.strings.format('{}', m)

    timestring = tf.strings.join(
        [timeformat(hour), timeformat(minute), timeformat(second)], separator=':')
    tf.print('=========='*8+timestring)
