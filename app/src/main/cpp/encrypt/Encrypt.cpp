//
// Created by stone on 2021/7/17.
//

#include <jni.h>
#include "../JniUtil.h"

/*
 *  加密 encode() 正常执行；
 *  解密 decode() 执行就崩溃。。。。。
 */

jobject secretKey;

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_stone_study_ndk_ui_encrypt_EncryptActivity_encode(JNIEnv *env, jobject thiz,
                                                           jbyteArray textAry) {
    /*
     * 打开 javax.crypto.Cipher ，查看到，支持的 加密算法对应的 android api level 。
     * AES-GCM
     */

    /*
     * cipher = Cipher.getInstance(KEY_ALGORITHM);
		//KeyGenerator 生成aes算法密钥
		secretKey = KeyGenerator.getInstance(KEY_ALGORITHM).generateKey();
		System.out.println("密钥的长度为：" + secretKey.getEncoded().length);

		cipher.init(Cipher.ENCRYPT_MODE, secretKey);//使用加密模式初始化 密钥
		byte[] encrypt = cipher.doFinal(str.getBytes()); //按单部分操作加密或解密数据，或者结束一个多部分操作。

		System.out.println("method1-加密：" + Arrays.toString(encrypt));
		cipher.init(Cipher.DECRYPT_MODE, secretKey);//使用解密模式初始化 密钥
		byte[] decrypt = cipher.doFinal(encrypt);
		System.out.println("method1-解密后：" + new String(decrypt));
————————————————
版权声明：本文为CSDN博主「匆忙拥挤repeat」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/jjwwmlp456/article/details/20960029
     */
    jclass cipherClz = env->FindClass("javax/crypto/Cipher");
    jmethodID getCipherInstanceMid = env->GetStaticMethodID(cipherClz, "getInstance", "(Ljava/lang/String;)Ljavax/crypto/Cipher;");
    jstring cipherAlgorithm = env->NewStringUTF("AES/ECB/PKCS5Padding");
    jobject cipherObj = env->CallStaticObjectMethod(cipherClz, getCipherInstanceMid, cipherAlgorithm);

    jclass keyGeneratorClz = env->FindClass("javax/crypto/KeyGenerator");
    jmethodID getKeyGenInstanceMid = env->GetStaticMethodID(keyGeneratorClz, "getInstance", "(Ljava/lang/String;)Ljavax/crypto/KeyGenerator;");
    jstring algorithm = env->NewStringUTF("AES");
    jobject keyGenObj = env->CallStaticObjectMethod(keyGeneratorClz, getKeyGenInstanceMid, algorithm);

    jmethodID generateKeyMid = env->GetMethodID(keyGeneratorClz, "generateKey", "()Ljavax/crypto/SecretKey;");
    if (!secretKey) {
        secretKey = env->CallObjectMethod(keyGenObj, generateKeyMid); // 生成密钥
    }

    jmethodID cipherInitMid = env->GetMethodID(cipherClz, "init", "(ILjava/security/Key;)V");
    env->CallVoidMethod(cipherObj, cipherInitMid, 1, secretKey); // 使用密钥 加密

    jmethodID cipherDoFinalMid = env->GetMethodID(cipherClz, "doFinal", "([B)[B");
    auto encrypt = env->CallObjectMethod(cipherObj, cipherDoFinalMid, textAry);

    env->DeleteLocalRef(cipherAlgorithm);
    env->DeleteLocalRef(algorithm);
    return static_cast<jbyteArray>(encrypt);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_stone_study_ndk_ui_encrypt_EncryptActivity_decode(JNIEnv *env, jobject thiz,
                                                           jbyteArray textAry) {
    if (!secretKey) {
        return nullptr;
    }

    jclass cipherClz = env->FindClass("javax/crypto/Cipher");
    jmethodID getCipherInstanceMid = env->GetStaticMethodID(cipherClz, "getInstance", "(Ljava/lang/String;)Ljavax/crypto/Cipher;");
    jstring cipherAlgorithm = env->NewStringUTF("AES/ECB/PKCS5Padding");
    jobject cipherObj = env->CallStaticObjectMethod(cipherClz, getCipherInstanceMid, cipherAlgorithm);

    jmethodID cipherInitMid = env->GetMethodID(cipherClz, "init", "(ILjava/security/Key;)V");
    env->CallVoidMethod(cipherObj, cipherInitMid, 2, secretKey); // 使用密钥 解密

    jmethodID cipherDoFinalMid = env->GetMethodID(cipherClz, "doFinal", "([B)[B");
    auto decrypt = env->CallObjectMethod(cipherObj, cipherDoFinalMid, textAry);

    env->DeleteLocalRef(cipherAlgorithm);
    return static_cast<jbyteArray>(decrypt);
}