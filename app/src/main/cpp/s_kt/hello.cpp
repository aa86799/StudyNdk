//
// Created by austin stone on 2021/6/27.
//

#include <jni.h>

extern "C" JNIEXPORT jint JNICALL
Java_com_stone_study_ndk_ui_jni_JniActivity_putAndGet(JNIEnv *env, jobject thiz, jint arg) {
    arg %= 2;
    return arg;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_stone_study_ndk_ui_jni_JniActivity_putDown(JNIEnv *env, jobject thiz, jint arg) {
    jclass clz = env->GetObjectClass(thiz);
    //最后一个参数为方法签名。见 https://stone.blog.csdn.net/article/details/89291328
    jmethodID mid = env->GetMethodID(clz, "whoLaLa", "(I)V");
    env->CallVoidMethod(thiz, mid, arg + 1);
    return arg;
}