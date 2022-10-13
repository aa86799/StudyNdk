//
// Created by stone on 2022/9/22.
//

#include "mp3_encoder.h"

Mp3Encoder::Mp3Encoder() {

}

Mp3Encoder::~Mp3Encoder() {
    destroy();
}

int Mp3Encoder::init(const char *pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate) {
    int ret = -1;
    this->pcmFile = fopen(pcmFilePath, "rb"); // 读
    if (pcmFile) {
        this->mp3File = fopen(mp3FilePath, "wb"); // 写
        if (mp3File) {
            this->lameClient = lame_init();
            lame_set_in_samplerate(lameClient, sampleRate);
            lame_set_out_samplerate(lameClient, sampleRate);
            lame_set_num_channels(lameClient, channels);
            lame_set_brate(lameClient, bitRate / 1000);
            lame_init_params(lameClient);
            ret = 0;
        }
    }
    return ret;
}

void Mp3Encoder::encode() {
    int bufferSize = 1024 * 256;
    short* buffer = new short[bufferSize / 2]; // 用 short ，因 是16位
    short* leftBuffer = new short[bufferSize / 4];
    short* rightBuffer = new short[bufferSize / 4];
    unsigned char* mp3_buffer = new unsigned char[bufferSize];
    size_t readBufferSize = 0;
    // fread(buffer, size, count, FILE*)  每次从 FIlE 中读取 count 个元素，每个元素占用 size 个字节
    while ((readBufferSize = fread(buffer, 2, bufferSize / 2, pcmFile)) > 0) {
        for (int i = 0; i < readBufferSize; i++) {
            if (i % 2 == 0) {
                leftBuffer[i / 2] = buffer[i];
            } else {
                rightBuffer[i / 2] = buffer[i];
            }
        }
        size_t wroteSize = lame_encode_buffer(lameClient, (short
                                              int *) leftBuffer, (short int *) rightBuffer,
                                              (int)(readBufferSize / 2), mp3_buffer, bufferSize);
        fwrite(mp3_buffer, 1, wroteSize, mp3File);
    }
    delete[] buffer;
    delete[] leftBuffer;
    delete[] rightBuffer;
    delete[] mp3_buffer;
}

void Mp3Encoder::destroy() {
    if (pcmFile) {
        fclose(pcmFile);
    }
    if (mp3File) {
        fclose(mp3File);
        lame_close(lameClient);
    }
}

Mp3Encoder *encoder;
extern "C"
JNIEXPORT jint JNICALL
Java_com_stone_study_ndk_ui_mp3_Mp3EncodeActivity_init(JNIEnv *env, jobject thiz, jstring pcm_path, jint audio_channels, jint bit_rate,
                                                       jint sample_rate, jstring mp3_path) {
    const char *pcmPath = env->GetStringUTFChars(pcm_path, JNI_FALSE);
    const char *mp3Path = env->GetStringUTFChars(mp3_path, JNI_FALSE);
    encoder = new Mp3Encoder();
    int result = encoder->init(pcmPath, mp3Path, sample_rate, audio_channels, bit_rate);
    env->ReleaseStringUTFChars(pcm_path, pcmPath);
    env->ReleaseStringUTFChars(mp3_path, mp3Path);
    return result;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_stone_study_ndk_ui_mp3_Mp3EncodeActivity_encode(JNIEnv *env, jobject thiz) {
    if (encoder) {
        encoder->encode();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_stone_study_ndk_ui_mp3_Mp3EncodeActivity_destroy(JNIEnv *env, jobject thiz) {
    if (encoder) {
        encoder->destroy();
        delete encoder;
    }
}