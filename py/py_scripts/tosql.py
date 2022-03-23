#!/usr/bin/python
#coding=utf-8

"""
工具类
"""
import shutil
import os,sys
import codecs
import json
import util

try:
    from ConfigParser import ConfigParser
except ImportError:
    from configparser import ConfigParser

class KeepCaseConfigParser(ConfigParser):
    def __init__(self,defaults=None):
        ConfigParser.__init__(self,defaults=None)
    def optionxform(self, optionstr):
        return optionstr

#获取当前路径
cur_path = os.path.abspath(os.curdir)

#获取当前路径的父目录
parent_path = os.path.abspath(os.path.join(cur_path, ".."))

conf_ini_file = './tosql.ini'

#读取conf.ini文件内容
tosql_ini = KeepCaseConfigParser()
with codecs.open(conf_ini_file, 'r', encoding='utf8') as cp_fd:
    tosql_ini.readfp(cp_fd)

#组织结构节点名
desc_name = 'Desc'

def ToArray(array):
    return [ i for i in array]

def LoadJson(filename):
    file = codecs.open(filename, 'rb')
    file_data = file.read()
    
    return json.loads(file_data)

def ToTxt(field):
    return '"' + str(field) + '"'

def OutputSqlDir(filename, sub_dir):
    dir = os.path.join(parent_path, 'sql', sub_dir)
    os.makedirs(dir, exist_ok=True)
    return os.path.join(dir, filename)

def DoInsertNode(op, node_name):
    src_file = tosql_ini.get(node_name, "SrcFile")
    #print(src_file)
    dest_file = tosql_ini.get(node_name, "DestFile")
    dest_table = tosql_ini.get(node_name, "DestTable")
    src_field = ToArray(tosql_ini.get(node_name, "SrcField").split(','))
    dest_field = ToArray(tosql_ini.get(node_name, "DestField").split(','))

    src_file = os.path.join(parent_path, 'tmp', src_file)
    json_data = util.load_json_conf(src_file)

    dest_filename = op + '_' + dest_file
    dest_file = open(OutputSqlDir(dest_filename, op), 'w', encoding='utf8')

    for item in json_data:
        sql_head = op + ' into ' + dest_table + ' set '
        sep = ''
        sql_body = ''
        for i, field in enumerate(src_field):
            if field in item:
                sql_body += sep + dest_field[i] + ' = ' + ToTxt(item[field])
                sep = ','
        sql = sql_head + sql_body + ';\n'
        sql = sql.replace('\'', '\\\"')
        dest_file.write(sql)
    dest_file.close()

def DoUpdateNode(op, node_name):
    pass

def DoDeleteNode(op, node_name):
    dest_file = tosql_ini.get(node_name, "DestFile")
    dest_table = tosql_ini.get(node_name, "DestTable")

    dest_filename = op + '_' + dest_file
    dest_file = open(OutputSqlDir(dest_filename, op), 'w', encoding='utf8')

    sql = op + ' from ' + dest_table + ';\n'
    dest_file.write(sql)
    dest_file.close()

op_hanlder_map = {
    "insert":DoInsertNode,
    "update":DoUpdateNode,
    "delete":DoDeleteNode
}

def DoProxy(node_name):
    op = tosql_ini.get(node_name, "OP")
    op_hanlder_map[op](op, node_name)

def Execute():
    for item_name in [i.strip() for i in tosql_ini.options(desc_name)]:
        for node_name in tosql_ini.get(desc_name, item_name).split(','):
            DoProxy(node_name)
