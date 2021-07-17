//
// Created by austin stone on 18/10/2017.
//

#ifndef NDK_TEST_LOGUTIL_H
#define NDK_TEST_LOGUTIL_H

#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <stdlib.h>
#include <exception>

#ifndef LOG_TAG
#define LOG_TAG "stone.stone"
#define slogd(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif


const char* jstringToCharString(JNIEnv*, jstring);

jstring charToJstring(JNIEnv*, const char*);

#endif //NDK_TEST_LOGUTIL_H
