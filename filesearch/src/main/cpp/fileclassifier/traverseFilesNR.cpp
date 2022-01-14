#include "classifier.h"
#include "../utils/FileUtils.h"

/**
 * 文件遍历的方法，非递归，请在java子线程中执行
 * @param path 文件的路径
 * @param paramsPtr
 * @param dealFunc
 */
void traverse(char *path, ParamsPtr paramsPtr, void(*dealFunc)(char *, char *, ParamsPtr)) {
    queue<string> mQueue;
    mQueue.push(path);
    while (!mQueue.empty()) {
        string currentPath = mQueue.front();
        mQueue.pop();
        DIR *dir = opendir(currentPath.c_str());
        if (dir == NULL) {
            perror("open dir error");
            continue;
        }
        Dirent ptr;
        while ((ptr = readdir(dir)) != NULL) {
            char *cur;//如果路劲过长加上文件名过长可能导致bug
            if (strcmp(ptr->d_name, ".") == 0 ||
                strcmp(ptr->d_name, "..") == 0)//current dir or parent dir
                continue;
            else if (ptr->d_type == 8 || ptr->d_type == 4) {//file or linked file
                //得到当前文件路径
                //为什么是2? 一个'/'加上一个'\0'
                int strLen = strlen(currentPath.c_str()) + strlen(ptr->d_name) + 2;
                cur = new char[strLen];
                memset(cur, '\0', sizeof(cur));
                sprintf(cur, "%s/%s", currentPath.c_str(), ptr->d_name);
                if (ptr->d_type == 8) {
                    dealFunc(cur, ptr->d_name, paramsPtr);
                } else {
                    if (strlen(ptr->d_name) != 0 && ptr->d_name[0] != '.')
                        mQueue.push(cur);
                }
            }
            delete cur;
        }
        closedir(dir);
    }
}

/**
 * 统计文件的数量
 * @param path 文件的路径
 * @param statisticsType 统计的类型
 * @return 统计文件的数量
 */
int statisticsFilesCountNR(char *path, int statisticsType) {
    queue<string> mQueue;
    mQueue.push(path);
    int fileCounts = 0, fileAndDirCounts = 0;
    while (!mQueue.empty()) {
        string currentPath = mQueue.front();
        mQueue.pop();
        DIR *dir = opendir(currentPath.c_str());
        if (dir == NULL) {
            perror("open dir error");
            continue;
        }
        Dirent ptr;
        while ((ptr = readdir(dir)) != NULL) {
            char *cur;//如果路劲过长加上文件名过长可能导致bug
            if (strcmp(ptr->d_name, ".") == 0 ||
                strcmp(ptr->d_name, "..") == 0)//current dir or parent dir
                continue;
            else if (ptr->d_type == 8 || ptr->d_type == 4) {//file or linked file
                //得到当前文件路径
                //为什么是2? 一个'/'加上一个'\0'
                int strLen = strlen(currentPath.c_str()) + strlen(ptr->d_name) + 2;
                cur = new char[strLen];
                memset(cur, '\0', sizeof(cur));
                sprintf(cur, "%s/%s", currentPath.c_str(), ptr->d_name);
                if (ptr->d_type == 4) {
                    mQueue.push(cur);
                } else {
                    ++fileCounts;
                }
                fileAndDirCounts++;
            }
            delete cur;
        }
        closedir(dir);
    }
    switch (statisticsType) {
        case STATISTICS_TYPE_FILES_AND_DIRS:
            return fileAndDirCounts;
        case STATISTICS_TYPE_ONLY_FILES:
        default:
            return fileCounts;
    }
}


/**
 * 遍历文件遍历到文件时的处理回调方法实现
 * @param path 路径
 * @param name 文件名
 * @param paramsPtr 文件处理通用结构体指针
 */
void dealTraveralResult(char *path, char *name, ParamsPtr paramsPtr) {
    if (name == NULL || strlen(name) == 0 || name[0] == '.')//illegal,illegal,hidden file
        return;
    JNIEnv *env = paramsPtr->env;
    int type = getFileType(paramsPtr->trie, name, true);
    bool acceptThisType = paramsPtr->accpetTypes(type, paramsPtr->acceptTypeArray,paramsPtr->acceptTypeCount);
    if (!acceptThisType)
        return;
    //LOGD("dealTraveralResult:%s,type:%d",path,type);
    if (paramsPtr->returnFile) {
        //todo: return file list to java
        jobject file_obj = env->NewObject(paramsPtr->file_clazz,
                                          paramsPtr->file_construct_mid,
                                          env->NewStringUTF(path));
        env->CallBooleanMethod(paramsPtr->listObject, paramsPtr->list_add_mid,
                               file_obj);
    } else {
        //todo: return fileInfo list to java
        jobject fileinfo_obj = env->NewObject(paramsPtr->file_info_clazz,
                                              paramsPtr->file_info_construct_mid,
                                              env->NewStringUTF(path),
                                              env->NewStringUTF(name), type);
        env->CallBooleanMethod(paramsPtr->listObject, paramsPtr->list_add_mid,
                               fileinfo_obj);
    }
}


/**
 * 用于文件夹中的文件的尺寸获取,作为一个函数指针使用
 */
void getFileSize(char *path, char *name, ParamsPtr paramsPtr) {
    paramsPtr->filesize += getFileSize(path);
}