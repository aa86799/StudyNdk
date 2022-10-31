////
//// Created by stone on 2022/9/22.
////
//
//#ifndef STUDYNDK_DECODE_PCM_YUV_H
//#define STUDYNDK_DECODE_PCM_YUV_H
//
//#include <jni.h>
//#include <cstdio>
//extern "C" { // ffmpeg 是 c 语言编写的。使用 c 的方式依赖。 c 和 cpp 编译后的函数签名是不同的
//#include "libavcodec/avcodec.h"
//#include "include/libavformat/avformat.h"
//#include "include/libswscale/swscale.h"
//#include "include/libswresample/swresample.h"
//#include "include/libavutil/pixdesc.h"
//}
//
//#include "../JniUtil.h"
//
//class PcmYuvDecoder {
//    FILE* videoFile{};
//    void handleVideoFrame();
//    void handleAudioFrame();
//public:
//    PcmYuvDecoder();
//    ~PcmYuvDecoder();
//    int init(); // 初始化F
//    int decodePcm(const char* videoFilePath);
//    int decodeYuv(const char* videoFilePath);
//    void release(); // 释放资源
//};
//
//#endif //STUDYNDK_DECODE_PCM_YUV_H
