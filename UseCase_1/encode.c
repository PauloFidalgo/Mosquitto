#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>

#define INPUT_FILE "/Users/wagner/Desktop/INESC/Converge/video_1.mp4"
#define OUTPUT_FILE "output_video.h264"

int main() {
    AVFormatContext *input_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVCodec *codec = NULL;
    AVFrame *frame = NULL;
    AVPacket packet;
    int ret, i;

    // Open input file
    ret = avformat_open_input(&input_ctx, INPUT_FILE, NULL, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error opening input file: %s\n", av_err2str(ret));
        return 1;
    }

    // Find stream info
    ret = avformat_find_stream_info(input_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error finding stream info: %s\n", av_err2str(ret));
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Find video stream
    int video_stream_index = -1;
    for (i = 0; i < input_ctx->nb_streams; i++) {
        if (input_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    if (video_stream_index == -1) {
        fprintf(stderr, "No video stream found in input file\n");
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Find decoder for the video stream
    codec = avcodec_find_decoder(input_ctx->streams[video_stream_index]->codecpar->codec_id);
    if (!codec) {
        fprintf(stderr, "Unsupported codec\n");
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Allocate codec context
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        fprintf(stderr, "Failed to allocate codec context\n");
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Initialize codec context
    ret = avcodec_parameters_to_context(codec_ctx, input_ctx->streams[video_stream_index]->codecpar);
    if (ret < 0) {
        fprintf(stderr, "Failed to initialize codec context: %s\n", av_err2str(ret));
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Open codec
    ret = avcodec_open2(codec_ctx, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Failed to open codec: %s\n", av_err2str(ret));
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Initialize frame
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Failed to allocate frame\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Initialize packet
    av_init_packet(&packet);
    packet.data = NULL;
    packet.size = 0;

    // Open output file
    FILE *output_file = fopen(OUTPUT_FILE, "wb");
    if (!output_file) {
        fprintf(stderr, "Error opening output file\n");
        av_frame_free(&frame);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&input_ctx);
        return 1;
    }

    // Read frames and encode
    while (av_read_frame(input_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream_index) {
            ret = avcodec_send_packet(codec_ctx, &packet);
            if (ret < 0) {
                fprintf(stderr, "Error sending packet for decoding: %s\n", av_err2str(ret));
                break;
            }
            while (ret >= 0) {
                ret = avcodec_receive_frame(codec_ctx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    fprintf(stderr, "Error during decoding: %s\n", av_err2str(ret));
                    break;
                }

                // Write encoded frame to output file
                fwrite(frame->data[0], 1, frame->linesize[0], output_file);
                fwrite(frame->data[1], 1, frame->linesize[1], output_file);
                fwrite(frame->data[2], 1, frame->linesize[2], output_file);
            }
        }
        av_packet_unref(&packet);
    }

    // Flush decoder
    ret = avcodec_send_packet(codec_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error flushing decoder: %s\n", av_err2str(ret));
    }
    while (ret >= 0) {
        ret = avcodec_receive_frame(codec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        } else if (ret < 0) {
            fprintf(stderr, "Error during decoding: %s\n", av_err2str(ret));
            break;
        }

        // Write encoded frame to output file
        fwrite(frame->data[0], 1, frame->linesize[0], output_file);
        fwrite(frame->data[1], 1, frame->linesize[1], output_file);
        fwrite(frame->data[2], 1, frame->linesize[2], output_file);
    }

    // Clean up
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&input_ctx);
    fclose(output_file);

    return 0;
}
