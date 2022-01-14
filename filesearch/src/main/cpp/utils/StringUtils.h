#ifndef FILECLASSIFIER_STRINGUTILS_H
#define FILECLASSIFIER_STRINGUTILS_H

#include "../common/Common.h"
#include <algorithm>

#define UP_TO_LOW_OFFSET 32

jstring str2jstring(JNIEnv *env, const char *pat);

string jstring2str(JNIEnv *env, jstring jstr);

char *jstring2chars(JNIEnv *env, jstring jstr);

void toLowerCase(char *str);

void toLowerCase(string str);

void toUpperCase(char *str);

void toUpperCase(string str);

#define JSTR2CHS(jstr) jstring2chars(env,jstr)

#endif
