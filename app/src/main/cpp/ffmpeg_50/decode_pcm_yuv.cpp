////
//// Created by stone on 2022/9/22.
////
//
//#include "decode_pcm_yuv.h"
//
//PcmYuvDecoder::PcmYuvDecoder() {
//
//}
//
//PcmYuvDecoder::~PcmYuvDecoder() {
//    release();
//}
//
//int PcmYuvDecoder::init() {
//    av_log_set_level(AV_LOG_INFO);
//
//    int ret = -1;
////    ret = avformat_network_init(); // 使用旧的GnuTLS或OpenSSL库，则需要调用
//    // https://ffmpeg.org/doxygen/5.0/group__libavf.html   4.0和5.0文档中，能看到  5.0中已经没有需要 调用 av_register_all()的描述了
////    ret = av_register_all(); // 初始化libavformat和注册所有的复用/解复用器和协议. 4.0中过时；5.0中已移除该 api
//
//
//    return ret = 1;
//}
//
//int custom_interrupt_callback(void *arg) {
//
//}
//
//int PcmYuvDecoder::decodePcm(const char *videoFilePath) {
//    int ret = -1;
//
//    AVFormatContext *fmtCtx = avformat_alloc_context(); // 分配
//    AVIOInterruptCB interruptCb = {custom_interrupt_callback, nullptr}; // io 中断结构体 { 回调函数 }
//    fmtCtx->interrupt_callback = interruptCb;
//    avformat_open_input(&fmtCtx, videoFilePath, nullptr, nullptr); //打开输入流
//
//    // 寻找流
//    int videoStreamIndex = -1;
//    int audioStreamIndex = -1;
//    for (int i = 0; i < fmtCtx->nb_streams; ++i) {
//        AVStream *stream = fmtCtx->streams[i];
//        if (AVMEDIA_TYPE_VIDEO == stream->codecpar->codec_type) {
//            videoStreamIndex = i;
//        } else if (AVMEDIA_TYPE_AUDIO == stream->codecpar->codec_type) {
//            audioStreamIndex = i;
//        }
//    }
//
//    // 打开音频解码器
//    const AVCodec *aCodec = avcodec_find_decoder(fmtCtx->streams[audioStreamIndex]->codecpar->codec_id);
//    AVCodecContext *audioCodecCtx = avcodec_alloc_context3(aCodec);
//    if (!audioCodecCtx) {
//        sloge("%s", "找不到对应的音频解码器");
//        return ret = -2;
//    }
//    // avcodec_open2 打开编解码器，在编码与解码中都会用到
//    if ((ret = avcodec_open2(audioCodecCtx, aCodec, nullptr)) < 0) {
//        sloge("%s", "打开音频解码器失败");
//        return ret;
//    }
//
//    // 打开视频解码器
//    const AVCodec *vCodec = avcodec_find_decoder(fmtCtx->streams[videoStreamIndex]->codecpar->codec_id);
//    AVCodecContext *videoCodecCtx = avcodec_alloc_context3(vCodec);
//    if (!videoCodecCtx) {
//        sloge("%s", "找不到对应的视频解码器");
//        return ret = -3;
//    }
//    if ((ret = avcodec_open2(videoCodecCtx, vCodec, nullptr)) < 0) {
//        sloge("%s", "打开视频解码器失败");
//        return ret;
//    }
//
//    // 构建音频的格式转换对象，及解码后数据存放的对象
//    SwrContext *sCtx = nullptr;
//    AVFrame *audioFrame = nullptr;
//    if (audioCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16) {
//        // 输出的声道：CHANNEL_IN_MONO为单声道，CHANNEL_IN_STEREO为双声道
//        sCtx = swr_alloc_set_opts(nullptr, AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_S16, 44100,
//                                  audioCodecCtx->channel_layout, audioCodecCtx->sample_fmt,
//                                  audioCodecCtx->sample_rate, 0, nullptr);
//        if (!sCtx || swr_init(sCtx)) {
//            if (sCtx) {
//                swr_free(&sCtx);
//            }
//            sloge("%s", "sCtx == null, 或 swr_init failed");
//            return ret = -4;
//        }
////        avcodec_alloc_frame(); // 过时 替代 av_frame_alloc()
//        audioFrame = av_frame_alloc();
//        if (!audioFrame) {
//            sloge("%s", "audioFrame 初始化失败");
//            return ret = -5;
//        }
//    }
//
//    // 构建视频的格式转换对象，及解码后数据存放的对象
////    avpicture_alloc(); // https://ffmpeg.org/doxygen/4.4/deprecated.html  能查到 该函数unused. Class AVPicture 过时了，用AVFrame 替代
////    AVPicture *picture;
////    bool pictureValid = avpicture_alloc(&picture, AV_PIX_FMT_YUV420P, videoCodecCtx->width, videoCodecCtx->height) == 0;
//
//
//    SwsContext *swsContext = nullptr;
//    swsContext = sws_getCachedContext(swsContext, videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt,
//                                      videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR,
//                                      nullptr, nullptr, nullptr);
//    AVFrame *videoFrame = av_frame_alloc();
//
//    //  读取流内容，并解码
//    AVPacket packet;
//    int gotFrame = 0;
//    while (true) {
//        if (av_read_frame(fmtCtx, &packet)) {
//            break;
//        }
//        int packetStreamIndex = packet.stream_index;
//        if (packetStreamIndex == audioStreamIndex) {
//            // Global avcodec_decode_video2 (AVCodecContext *avctx, AVFrame *picture, int *got_picture_ptr, const AVPacket *avpkt)
//            // Use avcodec_send_packet() and avcodec_receive_frame().
////            int len = avcodec_decode_video2(videoCodecCtx, videoFrame, &gotFrame, &packet);
//            if (avcodec_send_packet(audioCodecCtx, &packet) < 0) {
//                sloge("%s", "avcodec_send_packet audio 失败");
//                return ret = -6;
//            }
//            int gotFrame = avcodec_receive_frame(audioCodecCtx, audioFrame);
////            int len = avcodec_decode_video2(videoCodecCtx, videoFrame, &gotFrame, &packet);
////            if(len < 0) {
////                break;
////            }
//            if (gotFrame) {
//                this->handleVideoFrame();
//            }
//        } else if (packetStreamIndex == videoStreamIndex) {
////            int len = avcodec_decode_audio4(audioCodecCtx, audioFrame, &gotFrame, &packet);
////            if(len < 0) {
////                break; }
//            if (avcodec_send_packet(videoCodecCtx, &packet) < 0) {
//                sloge("%s", "avcodec_send_packet audio 失败");
//                return ret = -7;
//            }
//            int gotFrame = avcodec_receive_frame(videoCodecCtx, videoFrame);
//            if (gotFrame) {
//                this->handleAudioFrame();
//            }
//        }
//    }
//
//    // 处理解码后的裸数据
//
//    return ret;
//}
//
//int PcmYuvDecoder::decodeYuv(const char *videoFilePath) {
//
//}
//
//void PcmYuvDecoder::handleAudioFrame() {
//
//}
//
//void PcmYuvDecoder::handleVideoFrame() {
//
//}
//
//void PcmYuvDecoder::release() {
//
//}