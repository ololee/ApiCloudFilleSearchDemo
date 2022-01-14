#ifndef FILECLASSIFIER_CLASSIFIER_H
#define FILECLASSIFIER_CLASSIFIER_H

#include "../common/Common.h"
#include "../trie/SuffixTrie.h"
#include "../utils/StringUtils.h"
#include "../utils/FileUtils.h"
#include "classifyType.h"

/*
 * 统计文件的策略：
 * STATISTICS_TYPE_ONLY_FILES 仅统计文件
 * STATISTICS_TYPE_FILES_AND_DIRS 统计文件和文件夹
 */
#define STATISTICS_TYPE_ONLY_FILES 1
#define STATISTICS_TYPE_FILES_AND_DIRS 2

typedef struct dirent *Dirent;

/**
 * 文件处理通用结构体
 * trie：前缀树指针
 * accpetTypes:接受文件类型的函数，参数分别为子类型，接受类型数组，接受类型数组长度
 * acceptTypeArray：接受类型数组
 * acceptTypeCount：接受类型数组长度
 * returnFile：返回文件列表还是FileInfo列表
 * file_clazz：文件类
 * file_info_clazz：FileInfo类
 * listObject：返回的列表对象
 * list_add_mid：列表的增加方法
 * file_info_construct_mid：文件信息构造函数的方法
 * file_construct_mid：文件构造函数的方法
 */
typedef struct {
    JNIEnv *env;
    SuffixTrie *trie;

    bool (*accpetTypes)(int, int *, int);//function pointer
    int *acceptTypeArray;
    int acceptTypeCount;
    bool returnFile;//true return file,false return fileinfo
    jclass file_clazz, file_info_clazz;
    jobject listObject;
    jmethodID list_add_mid, file_info_construct_mid, file_construct_mid;
    long long filesize;
} Params, *ParamsPtr;

/**
 * 遍历文件的类，在traverseFilesNR.cpp中实现
 * @param path 文件的路径
 * @param paramsPtr 文件处理通用结构体指针
 * @param dealFunc 遍历文件遍历到文件时的处理回调方法,参数含义分别为：路径、文件名、文件处理通用结构体指针
 */
void traverse(char *path, ParamsPtr paramsPtr, void(*dealFunc)(char *, char *, ParamsPtr));

/**
 * 遍历文件遍历到文件时的处理回调方法实现
 * @param path 路径
 * @param name 文件名
 * @param paramsPtr 文件处理通用结构体指针
 */
void dealTraveralResult(char *path, char *name, ParamsPtr paramsPtr);


/**
 * 统计文件的数量
 * @param path 文件的路径
 * @param statisticsType 统计的类型
 * @return 统计文件的数量
 */
int statisticsFilesCountNR(char *path, int statisticsType);


/**
 * 用于文件夹中的文件的尺寸获取,作为一个函数指针使用
 */
void getFileSize(char *path, char *name, ParamsPtr paramsPtr);

#endif