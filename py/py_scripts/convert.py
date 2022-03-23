#coding=utf-8
#!/usr/bin/python

"""
convertTable table 将excel表格转成配置文件
"""
import os
import sys
import util
import json
import types
import codecs
import traceback

xlrd = util.import_help('xlrd')

def convertTableXlsxFolder2CfgFile(source_path, dest_path):
    for item in os.listdir(source_path):
        eachXlsxPath = os.path.join(source_path, item)
        if util.is_endswith(item, ["xlsm", "xlsx"]) and not util.is_startswith(item, ["~"]):
            print ("convert table: ", item)
            convertTableXlsxFile2CfgFile(eachXlsxPath, dest_path)

def convertTableXlsxFile2CfgFile(filename, dest_path):
    data = xlrd.open_workbook(filename, 'utf8')
    sheets = data.sheets()
    for idx in range(len(sheets)):
        sheet = sheets[idx]
        convertTableXlsxTableToJsFile(filename, sheet, dest_path)

# 导出客户端配置js表格
def convertTableXlsxTableToJsFile(filename, sheet, to_dir):
    nrows = sheet.nrows #行号
    ncols = sheet.ncols #列号

    outfile_bodyname = os.path.basename(filename).split(".")[0]
    outfilename = outfile_bodyname + ".json"
    writefile = os.path.join(to_dir, outfilename)

    data = []
    dict = False
    #cellName = sheet.cell(1, 1).value
    cellType = sheet.cell(2, 1).value
    if cellType.lower() == 'key':
        data = {}
        dict = True

    for rowIndex in range(3, nrows): # 第一行为描述内容
        rowData = {}
        for columnIndex in range(1, ncols): # 第一列为描述内容
            cellName = sheet.cell(1, columnIndex).value
            dataType = sheet.cell(2, columnIndex).value
            name = cellName
            if dataType == "int":
                rowData[name] = int(sheet.cell(rowIndex, columnIndex).value)
            elif dataType == "bool":
                cellData = sheet.cell(rowIndex, columnIndex).value
                if cellData == "true":
                    rowData[name] = True
                else:
                    rowData[name] = False
            else:
                rowData[name] = sheet.cell(rowIndex, columnIndex).value
        if dict:
            key = sheet.cell(rowIndex, 1).value
            data[key] = rowData
        else:
            data.append(rowData)

    jsonData = {}
    jsonData[outfilename] = data 
    encodedjson = json.dumps(data, ensure_ascii=False, sort_keys=True, indent=2)
    #encodedjson = encodedjson.encode("UTF-8") #如果不自己设定字符集,会把它按照当前环境的默认编码来编码,命令行里是ASCII,ECLLIPSE里是UTF-8.在命令行就无法encode中文了,报错

    encodedjson = encodedjson.replace('\\"', '"')
    encodedjson = encodedjson.replace('"[', '[')
    encodedjson = encodedjson.replace(']"', ']')
    encodedjson = encodedjson.replace('"{', '{')
    encodedjson = encodedjson.replace('}"', '}')

    writefile = os.path.join(to_dir, outfilename)

    json_file = open(writefile, "w", encoding='utf8')
    json_file.write(encodedjson)
    json_file.close()

# 转数据档
def convertData(source_path, dest_path):
    convertTableXlsxFolder2CfgFile(source_path, dest_path)

def runConvert(datapath):
    dest_path = os.path.abspath(os.path.join(datapath, 'tmp'))
    if not os.path.exists(dest_path):
        os.mkdir(dest_path)
    convertData(datapath, dest_path)

    return dest_path

def ConvertTable():
    tmp = os.path.join(os.curdir, "..")
    runConvert(os.path.abspath(tmp))
