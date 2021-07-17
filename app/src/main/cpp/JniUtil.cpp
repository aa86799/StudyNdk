//
// Created by austin stone on 18/10/2017.
//

#include "JniUtil.h"


extern const char* jstringToCharString(JNIEnv* env, jstring jstr) {
//    jclass clsstring = env->FindClass("java/lang/String");
//    jstring strencode = env->NewStringUTF("utf-8");
//    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
//    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
//    jsize alen = env->GetArrayLength(barr);
//    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
//    char* rtn = NULL;
//    if (alen > 0) {
//        rtn = (char*)malloc(alen + 1);
//
//        memcpy(rtn, ba, alen);
//        rtn[alen] = 0;
//    }
//    env->ReleaseByteArrayElements(barr, ba, 0);
//    return rtn;

    /*
     * GetStringUTFChars 创建一个Unicode字符串
     * 该函数，为java.lang.String 要分配足够的内存；
     * 若分配失败，会抛出一个 OutOfMemoryError 异常。
     *
     * 关于函数最后一个参数：jboolean *isCopy;  可以直接传NULL
     *
     * 返回的字符串，使用完毕后，记得使用ReleaseStringUTFChars 释放其占用的jvm内存
     */
//    return env->GetStringUTFChars(jstr, JNI_FALSE);

    return env->GetStringUTFChars(jstr, NULL);
}

jstring charToJstring(JNIEnv* env, const char* pat) {
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    /*
     * 如不能为构造 java.lang.String 分配足够的内存，NewStringUTF会抛出一个OutOfMemoryError异常，并返回一个 NULL
     */
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

jsize getJStringLen(JNIEnv *env, jstring str) {
    /*
     * UTF-8 字符串以’\0’结尾，而 Unicode 字符串不是。
     * 如果 jstring 指向一个 Unicode 编码的字符串，为了得到这个字符串的长度，可以调用 GetStringLength。
     * 如果一个 jstring 指向一个 UTF-8 编码的字符串，为了得到这个字符串的字节长 度，可以调用标准 C 函数 strlen。
     * 或者直接对 jstring 调用 JNI 函数 GetStringUTFLength，而不用管 jstring 指向的字符串的编码格式。
     */
//    env->GetStringLength(str);
//    strlen(char *);//
    return env->GetStringUTFLength(str);
}
