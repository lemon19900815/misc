#!/usr/bin/python
#coding=utf-8

"""
工具类
"""
import os,sys
import json
import codecs
import tarfile
import traceback

def is_endswith(s, items):
    for item in items:
        if s.endswith(item):
            return True
    return False

def is_startswith(s, items):
    for item in items:
        if s.startswith(item):
            return True
    return False

# 加载json配置
def load_json_conf(filename):
    file = codecs.open(filename, 'rb')
    return json.loads(file.read())

# 导入模块
def import_help(module_name):
    while True:
        try:
            module = __import__(module_name)
            return module
        except:
            print('初始化模块: ', module_name)
            os.system('pip install %s' % module_name)

# 打包文件夹tar.gz
def make_targz(output_filename, source_dir):
  with tarfile.open(output_filename, "w:gz") as tar:
    tar.add(source_dir, arcname=os.path.basename(source_dir))

# 交互式等待输入选项卡内容
def interactive_input(options):
    opt_list = [opt for opt in options]

    while True:
        for i, opt in enumerate(opt_list):
            print(i, opt)
        try:
            seqno = input('输入序号: ')
            seqno = int(seqno)
            if seqno >= len(opt_list):
                print('无效序号：', seqno)
                continue
            return opt_list[seqno]
        except:
            print('序号错误：', seqno)
            traceback.print_exc()
    return None
