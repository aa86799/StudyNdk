//
// Created by stone on 2022/10/14.
//

#include <jni.h>
#include "../JniUtil.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/log.h"
}

static int get_audio_obj_type(int aactype){
    //AAC HE V2 = AAC LC + SBR + PS
    //AAV HE = AAC LC + SBR
    //所以无论是 AAC_HEv2 还是 AAC_HE 都是 AAC_LC
    switch(aactype){
        case 0:
        case 2:
        case 3:
            return aactype+1;
        case 1:
        case 4:
        case 28:
            return 2;
        default:
            return 2;

    }
}

static int get_channel_config(int channels, int aactype){
    //如果是 AAC HEv2 通道数减半
    if(aactype == 28){
        return (channels / 2);
    }
    return channels;
}

static int get_sample_rate_index(int freq, int aactype){

    int i = 0;
    int freq_arr[13] = {
            96000, 88200, 64000, 48000, 44100, 32000,
            24000, 22050, 16000, 12000, 11025, 8000, 7350
    };

    //如果是 AAC HEv2 或 AAC HE, 则频率减半
    if(aactype == 28 || aactype == 4){
        freq /= 2;
    }

    for(i=0; i< 13; i++){
        if(freq == freq_arr[i]){
            return i;
        }
    }
    return 4;//默认是44100
}

static void adts_header(char *szAdtsHeader, int dataLen, int aactype, int frequency, int channels){

    int audio_object_type = get_audio_obj_type(aactype);
    int sampling_frequency_index = get_sample_rate_index(frequency, aactype);
    int channel_config = get_channel_config(channels, aactype);

    printf("aot=%d, freq_index=%d, channel=%d\n", audio_object_type, sampling_frequency_index, channel_config);

    int adtsLen = dataLen + 7;

    szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
    szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
    szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
    szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits
    szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

    szAdtsHeader[2] = (audio_object_type - 1)<<6;            //profile:audio_object_type - 1                      2bits
    szAdtsHeader[2] |= (sampling_frequency_index & 0x0f)<<2; //sampling frequency index:sampling_frequency_index  4bits
    szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
    szAdtsHeader[2] |= (channel_config & 0x04)>>2;           //channel configuration:channel_config               高1bit

    szAdtsHeader[3] = (channel_config & 0x03)<<6;     //channel configuration:channel_config      低2bits
    szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
    szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
    szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit
    szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
    szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

    szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
    szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
    szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
    szAdtsHeader[6] = 0xfc;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_stone_study_ndk_ui_ffmpeg_FFMpegActivity_deleteAndMoveFile(JNIEnv *env, jobject thiz, jstring file_path) {
    //    在ffmpeg V4.2版本中使用的是 avpriv_io_delete avpriv_io_move

/*
 * 在ffmpeg V4.4版本中使用的是 ffurl_delete ffurl_move ，这两个函数仅存在于 avio.c ，而没有在 avio.h 中声明
 * 修改libavformat/libavformat.v 文件，
 * LIBAVFORMAT_MAJOR {
    global:
        av*;
        ffurl*;
    local:
        *;
};
 再重新编译生成库文件。
 由于本身没有在头文件中声明， 需要在对应头文件中声明，不然编译会出错
 */

//    int ret = ffurl_delete(env->GetStringUTFChars(file_path, nullptr));
//    if (!ret) {
//        av_log(nullptr, AV_LOG_ERROR, "Failed to delete file");
//    }

    AVFormatContext *ctx = nullptr;
    const char* path = env->GetStringUTFChars(file_path, nullptr);
    int ret = avformat_open_input(&ctx, path, nullptr, nullptr);
    av_dump_format(ctx, 0, path, 0); // 打印信息 持续时间，比特率，流，容器，程序，元数据，侧数据, 编解码器和时基
    avformat_close_input(&ctx);

    return ret;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_stone_study_ndk_ui_ffmpeg_FFMpegActivity_test(JNIEnv *env, jobject thiz, jstring file_path, jstring out_path) {
    AVFormatContext *ctx = nullptr;
    const char* src_path = env->GetStringUTFChars(file_path, nullptr);
    const char* dst_path = env->GetStringUTFChars(out_path, nullptr);
    int ret = avformat_open_input(&ctx, src_path, nullptr, nullptr);
    avformat_find_stream_info(ctx, nullptr);

    FILE *dstFile = fopen(dst_path, "wb");
    // av_dump_format(ctx, 0, src_path, 0); // 打印信息 持续时间，比特率，流，容器，程序，元数据，侧数据, 编解码器和时基
    ret = av_find_best_stream(ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (!ret) {
        sloge("%s", "cant find the best stream");
        avformat_close_input(&ctx);
        fclose(dstFile);
        return ret;
    }
//    av_init_packet() // 过时
    AVPacket *pkt = nullptr;
    int audioIndex = ret;
    int aac_type = ctx->streams[audioIndex]->codecpar->profile;
    int channels = ctx->streams[audioIndex]->codecpar->channels;
    int sample_rate= ctx->streams[audioIndex]->codecpar->sample_rate;
    while (av_read_frame(ctx, pkt) >= 0) {
        if (pkt->stream_index == audioIndex) {
            char adts_header_buf[7];
            adts_header(adts_header_buf, pkt->size, aac_type, sample_rate, channels);
            fwrite(adts_header_buf, 1, 7, dstFile);

            auto len = fwrite(pkt->data, 1, pkt->size, dstFile);
            if (len != pkt->size) {
                // error
            }
        }
        av_packet_unref(pkt);
    }
    avformat_close_input(&ctx);

    return ret;
}
