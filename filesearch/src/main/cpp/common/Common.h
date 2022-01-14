#ifndef FILECLASSIFIER_COMMON_H
#define FILECLASSIFIER_COMMON_H
#include <jni.h>
#include <string>
#include <android/log.h>
#include <iostream>
#include <dirent.h>
#include <queue>
#include <list>
#include <sys/stat.h>
#include <map>

#define TAG "FileClassifierNative"
#define ENABLE_LOG true

#define LOGD(...) if(ENABLE_LOG)__android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) if(ENABLE_LOG)__android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) if(ENABLE_LOG)__android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) if(ENABLE_LOG)__android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) if(ENABLE_LOG)__android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))


#define FIND_CLASS(var, className) \
var = env->FindClass(className); \
if(!var){ \
 LOGE("could not find class %s",className); \
}

#define GET_METHOD_ID(var, clazz, methodName, descriptor) \
var = env->GetMethodID(clazz, methodName, descriptor); \
if(!var){ \
 LOGE("unabele to find field %s",methodName); \
}

#define GET_STATIC_METHOD_ID(var, clazz, methodName, descriptor) \
var = env->GetStaticMethodID(clazz, methodName, descriptor); \
if(!var){ \
 LOGE("unabele to find field %s",methodName); \
}


#define GET_FIELD_ID(var ,clazz, fieldName, SIG) \
var =  env->GetFieldID(clzzz,fieldName,SIG); \
if(!var){ \
 LOGE("unabele to find field %s",fieldName); \
}

#define ASSERT_NOT_NULL(var,message)                             \
  jclass nullPtrException;                                       \
  FIND_CLASS(nullPtrException, "java/lang/NullPointerException") \
   if (var == nullptr) {                                        \
      env->ThrowNew(nullPtrException,message);                   \
        return ;                                                 \
  }

#define ASSERT_NOT_NULL_NO_REPEAT(var,message)                  \
   if (var == nullptr) {                                        \
      env->ThrowNew(nullPtrException,message);                   \
        return ;                                                 \
  }

#define ASSERT_NOT_NULL_AND_RETURN(var,returnValue,message)      \
  jclass nullPtrException;                                       \
  FIND_CLASS(nullPtrException, "java/lang/NullPointerException") \
   if (var == nullptr) {                                        \
      env->ThrowNew(nullPtrException,message);                   \
        return returnValue;                                      \
  }

using namespace std;
#endif
