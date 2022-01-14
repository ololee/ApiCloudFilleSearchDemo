#include "StringUtils.h"


jstring str2jstring(JNIEnv *env, const char *pat) {
    jclass strClass;
    jmethodID ctorID;
    FIND_CLASS(strClass, "Ljava/lang/String;");
    GET_METHOD_ID(ctorID, strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    jstring encoding = (env)->NewStringUTF("GB2312");
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}


string jstring2str(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring;
    jmethodID mid;
    FIND_CLASS(clsstring, "java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    GET_METHOD_ID(mid, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    string stemp(rtn);
    free(rtn);
    return stemp;
}

char *jstring2chars(JNIEnv *env, jstring jstr) {
    return const_cast<char *>(env->GetStringUTFChars(jstr, NULL));
}

void toLowerCase(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += UP_TO_LOW_OFFSET;
        }
    }
}


void toLowerCase(string str) {
    transform(str.begin(), str.end(), str.end(), ::toupper);
}

void toUpperCase(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] -= UP_TO_LOW_OFFSET;
        }
    }
}

void toUpperCase(string str) {
    transform(str.begin(), str.end(), str.end(), ::toupper);
}

