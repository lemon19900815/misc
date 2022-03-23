#!/usr/bin/python
#coding=utf-8

"""
工具类
"""
import os,sys
import traceback
import json
import util

pymysql = util.import_help('pymysql')
import pymysql.cursors

# 创建db连接
def create_connection(conf):
    connection = pymysql.connect(host=conf["host"],
         user=conf["user"], password=conf["password"], db=conf["database"], charset='utf8')
    return connection

# 执行sql
# mysql_conf = {'user': , 'host': , 'database':}
def mysql_execute_sql(mysql_conf, sql):
    conn = create_connection(mysql_conf)
    cursor = conn.cursor()
    cursor.execute(sql)

    conn.commit()
    cursor.close()
    conn.close()
