//
// Created by stone on 2022/9/22.
//

#ifndef STUDYNDK_MP3_ENCODER_H
#define STUDYNDK_MP3_ENCODER_H

#include <jni.h>
#include <cstdio>
#include "libmp3lame/lame.h"

class Mp3Encoder {
    FILE* pcmFile{};
    FILE* mp3File{};
    lame_t lameClient{};
public:
    Mp3Encoder();
    ~Mp3Encoder();
    int init(const char* pcmFilePath, const char* mp3FilePath, int sampleRate, int channels, int bitRate);
    void encode();
    void destroy();

};

#endif //STUDYNDK_MP3_ENCODER_H
