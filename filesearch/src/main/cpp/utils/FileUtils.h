#ifndef FILECLASSIFIER_FILEUTILS_H
#define FILECLASSIFIER_FILEUTILS_H
#include "../common/Common.h"
#include "../trie/SuffixTrie.h"
#include "StringUtils.h"
/**
 * 获取路劲的后缀
 * @param path 路劲
 * @return 后缀
 */
char *getSuffix(char *path,bool toLower);

/**
 * 通过文件的全名获得文件类型
 * @param trie 前缀树
 * @param name 文件全名
 * @return
 */
int getFileType(SuffixTrie* trie,char* name,bool toLower);

/**
 * 通过文件的路径获得是否是目录
 * @param filePath
 * @return 是否目录
 */
bool isDirectory(string filePath);

/***
 * 获取文件的size
 */
unsigned long long getFileSize(char *path);
#endif
