#include "FileUtils.h"

/**
 * 获取路劲的后缀
 * @param path 路劲
 * @return 后缀
 */
char *getSuffix(char *path, bool toLower) {
    char *ret = strrchr(path, '.');
    if (ret) {
        if (toLower) {
            toLowerCase(ret + 1);
        }
        return ret + 1;
    }
    return "";
}

/**
 * 获得文件的类型
 * @param trie 前缀树
 * @param name 文件名称
 * @param toLower 后缀转为小写字母
 * @return
 */
int getFileType(SuffixTrie *trie, char *name, bool toLower) {
    char *suffix = getSuffix(name, toLower);
    if (strlen(suffix) == 0)
        return 0;
    return trie->search(suffix);
}

/**
 * 通过文件的路径获得是否是目录
 * @param filePath
 * @return 是否目录
 */
bool isDirectory(string filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

/***
 * 获取文件的size
 */
unsigned long long getFileSize(char *path) {
    long long result = 0;
    struct stat statbuff;
    if (stat(path, &statbuff) >= 0) {
        result = statbuff.st_size;
    }
    return result;
}

