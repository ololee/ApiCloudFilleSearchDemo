#include "classifier.h"

/**
 * 初始化前缀树
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_ololee_fileclassifier_FileClassifierHelper_initSuffixTrieNative(JNIEnv *env, jclass clazz,
                                                                        jobject suffix_trie_map) {
    jclass mapClazz, listClazz, setClazz, iteratorClazz, integerClazz, nullPtrException;
    FIND_CLASS(mapClazz, "java/util/Map")
    FIND_CLASS(listClazz, "java/util/List")
    FIND_CLASS(setClazz, "java/util/Set")
    FIND_CLASS(iteratorClazz, "java/util/Iterator")
    FIND_CLASS(integerClazz, "java/lang/Integer")
    FIND_CLASS(nullPtrException, "java/lang/NullPointerException")
    jmethodID mapKeySetMid, mapGetMid, setIteratorMid, iteratorHasNextMid, iteratorNextMid, listSizeMid, listGetMid, integerIntValMid;
    GET_METHOD_ID(mapKeySetMid, mapClazz, "keySet", "()Ljava/util/Set;")
    GET_METHOD_ID(mapGetMid, mapClazz, "get", "(Ljava/lang/Object;)Ljava/lang/Object;")
    GET_METHOD_ID(setIteratorMid, setClazz, "iterator", "()Ljava/util/Iterator;")
    GET_METHOD_ID(iteratorHasNextMid, iteratorClazz, "hasNext", "()Z")
    GET_METHOD_ID(iteratorNextMid, iteratorClazz, "next", "()Ljava/lang/Object;")
    GET_METHOD_ID(listSizeMid, listClazz, "size", "()I")
    GET_METHOD_ID(listGetMid, listClazz, "get", "(I)Ljava/lang/Object;")
    GET_METHOD_ID(integerIntValMid, integerClazz, "intValue", "()I")
    jobject suffixIntegerSets, integerIterator;
    suffixIntegerSets = env->CallObjectMethod(suffix_trie_map, mapKeySetMid);
    integerIterator = env->CallObjectMethod(suffixIntegerSets, setIteratorMid);
    SuffixTrie *trie = new SuffixTrie;
    while (env->CallBooleanMethod(integerIterator, iteratorHasNextMid)) {
        jobject key = env->CallObjectMethod(integerIterator, iteratorNextMid);
        jint keyInt = env->CallIntMethod(key, integerIntValMid);
        jobject values = env->CallObjectMethod(suffix_trie_map, mapGetMid, key);
        if (values == nullptr) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            env->ThrowNew(nullPtrException, "trie suffix must not be null.");
            delete trie;
            return;
        }
        jint valueSize = env->CallIntMethod(values, listSizeMid);
        for (int i = 0; i < valueSize; ++i) {
            jstring strValue = reinterpret_cast<jstring>(env->CallObjectMethod(values, listGetMid,
                                                                               i));
            LOGD("%d,%s", keyInt, JSTR2CHS(strValue));
            //todo initSuffixTrie
            trie->insert(JSTR2CHS(strValue), keyInt);
        }
    }
    jfieldID suffixTirePtrFid = env->GetStaticFieldID(clazz, "suffixTriePtr", "J");
    env->SetStaticLongField(clazz, suffixTirePtrFid, reinterpret_cast<jlong>(trie));
}


/**
 *  通过后缀获得文件的类型
 */
extern "C"
JNIEXPORT jint JNICALL
Java_cn_ololee_fileclassifier_FileClassifierHelper_getTypeBySuffixNative(JNIEnv *env, jclass clazz,
                                                                         jstring suffix) {
    jfieldID suffixTirePtrFid = env->GetStaticFieldID(clazz, "suffixTriePtr", "J");
    SuffixTrie *trie = reinterpret_cast<SuffixTrie *>(env->GetStaticLongField(clazz,
                                                                              suffixTirePtrFid));
    ASSERT_NOT_NULL_AND_RETURN(trie, 0,
                               "trie must not be null. Are you sure you had inited trie before use?")
    char *suffixChars = JSTR2CHS(suffix);
    if (strlen(suffixChars) == 0)
        return 0;
    int type = trie->search(suffixChars);
    env->ReleaseStringChars(suffix, reinterpret_cast<const jchar *>(suffixChars));
    return type;
}


/**
 * 获得划分的文件
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_ololee_fileclassifier_FileClassifierHelper_getFilesNative(JNIEnv *env, jclass clazz,
                                                                  jstring jpath,
                                                                  jobject file_classifier,
                                                                  jobject file_info_callback) {
    jclass file_classifier_clazz, file_info_clallback_clazz, list_clazz, file_info_clazz, file_clazz;
    file_classifier_clazz = env->GetObjectClass(file_classifier);
    file_info_clallback_clazz = env->GetObjectClass(file_info_callback);
    FIND_CLASS(list_clazz, "java/util/LinkedList")
    FIND_CLASS(file_info_clazz, "cn/ololee/fileclassifier/FileInfo")
    FIND_CLASS(file_clazz, "java/io/File")
    jmethodID file_classifier_use_mask_mid,
            file_classifier_parent_mask_mid,
            file_classifier_accept_types_mid,
            file_classifier_return_file_mid,
            file_info_callback_updateFileInfoList_mid,
            file_info_callback_updateFileList_mid,
            list_construct_mid,
            list_add_mid,
            file_info_construct_mid,
            file_construct_mid;
    ASSERT_NOT_NULL(file_classifier, "file_classifier must not null")
    GET_METHOD_ID(file_classifier_use_mask_mid, file_classifier_clazz, "useMask", "()Z")
    GET_METHOD_ID(file_classifier_parent_mask_mid, file_classifier_clazz, "mask", "()I")
    GET_METHOD_ID(file_classifier_accept_types_mid, file_classifier_clazz, "acceptTypes", "()[I")
    GET_METHOD_ID(file_classifier_return_file_mid, file_classifier_clazz, "returnFile", "()Z")
    GET_METHOD_ID(file_info_callback_updateFileInfoList_mid, file_info_clallback_clazz,
                  "updateFileInfoList", "(Ljava/util/List;)V")
    GET_METHOD_ID(file_info_callback_updateFileList_mid, file_info_clallback_clazz,
                  "updateFileList", "(Ljava/util/List;)V")
    GET_METHOD_ID(list_construct_mid, list_clazz, "<init>", "()V")
    GET_METHOD_ID(list_add_mid, list_clazz, "add", "(Ljava/lang/Object;)Z")
    GET_METHOD_ID(file_info_construct_mid, file_info_clazz, "<init>",
                  "(Ljava/lang/String;Ljava/lang/String;I)V")
    GET_METHOD_ID(file_construct_mid, file_clazz, "<init>", "(Ljava/lang/String;)V")
    Params params;
    ParamsPtr paramsPtr = &params;
    paramsPtr->env = env;
    paramsPtr->listObject = env->NewObject(list_clazz, list_construct_mid);
    paramsPtr->list_add_mid = list_add_mid;
    paramsPtr->file_info_clazz = file_info_clazz;
    paramsPtr->file_info_construct_mid = file_info_construct_mid;
    paramsPtr->file_construct_mid = file_construct_mid;
    paramsPtr->file_clazz = file_clazz;
    jfieldID suffixTirePtrFid = env->GetStaticFieldID(clazz, "suffixTriePtr", "J");
    paramsPtr->trie = reinterpret_cast<SuffixTrie *>(env->GetStaticLongField(clazz,
                                                                             suffixTirePtrFid));
    ASSERT_NOT_NULL_NO_REPEAT(paramsPtr->trie,
                              "trie must not be null. Are you sure you had inited trie before use?")
    jboolean useMask = env->CallBooleanMethod(file_classifier, file_classifier_use_mask_mid);
    jintArray acceptTypesArray =
            reinterpret_cast<jintArray>(env->CallObjectMethod(file_classifier,
                                                              file_classifier_accept_types_mid));
    jint *acceptTypesJintArray = env->GetIntArrayElements(acceptTypesArray, NULL);
    if (useMask == JNI_TRUE) {
        paramsPtr->accpetTypes = acceptTypesByMask;
        jint parentMask = env->CallIntMethod(file_classifier, file_classifier_parent_mask_mid);
        paramsPtr->acceptTypeArray = new int[]{parentMask, acceptTypesJintArray[0]};
    } else {
        paramsPtr->accpetTypes = acceptTypeByTraverse;
        paramsPtr->acceptTypeArray = acceptTypesJintArray;
        paramsPtr->acceptTypeCount = env->GetArrayLength(acceptTypesArray);
    }
    paramsPtr->returnFile = env->CallBooleanMethod(file_classifier,
                                                   file_classifier_return_file_mid);
    char *path = JSTR2CHS(jpath);
    traverse(path, paramsPtr, dealTraveralResult);
    env->CallVoidMethod(file_info_callback,
                        paramsPtr->returnFile ? file_info_callback_updateFileList_mid
                                              : file_info_callback_updateFileInfoList_mid,
                        paramsPtr->listObject);
    env->ReleaseStringUTFChars(jpath, path);
    jthrowable throwable = env->ExceptionOccurred();
    if (throwable != NULL)
        LOGD("some error happens");
}


/**
 * 统计文件的数量
 */
extern "C"
JNIEXPORT jint JNICALL
Java_cn_ololee_fileclassifier_FileClassifierHelper_getFileCountNative(JNIEnv *env, jclass clazz,
                                                                      jstring jpath,
                                                                      jint statistics_type) {
    char *path = JSTR2CHS(jpath);
    int count = statisticsFilesCountNR(path, statistics_type);
    env->ReleaseStringUTFChars(jpath, path);
    return count;
}

/**
  * 获取文件或者文件夹的大小(size)，比java快了1.5倍
  */
extern "C"
JNIEXPORT jlong JNICALL
Java_cn_ololee_fileclassifier_FileClassifierHelper_getFileSizeNative(JNIEnv *env, jclass clazz,
                                                                     jstring jpath) {
    char *path = JSTR2CHS(jpath);
    long long filesize;
    if (isDirectory(path)) {
        Params params;
        ParamsPtr paramsPtr = &params;
        paramsPtr->filesize = 0;
        traverse(path, paramsPtr, getFileSize);
        filesize = paramsPtr->filesize;
    } else {
        filesize = getFileSize(path);
    }
    env->ReleaseStringUTFChars(jpath, path);
    return filesize;
}

/**
  * 清除native层开辟的内存空间
  */
extern "C"
JNIEXPORT void JNICALL
Java_cn_ololee_fileclassifier_FileClassifierHelper_clearNative(JNIEnv *env, jclass clazz,
                                                               jlong suffix_tire_ptr) {
    if (suffix_tire_ptr != 0) {
        SuffixTrie *suffixTire = reinterpret_cast<SuffixTrie *>(suffix_tire_ptr);
        delete suffixTire;
    }
}