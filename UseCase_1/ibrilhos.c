#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *inputFileName = "video_1.mp4";
    const char *outputURL = "rtmp://your-rtmp-server.com/live/streamKey";
    const char *ffmpegCmd = "ffmpeg -re -i video_1.mp4 -c:v libx264 -preset ultrafast -tune zerolatency -crf 23 -c:a aac -b:a 128k -f flv outputURL";

    // Execute FFmpeg command
    int result = system(ffmpegCmd);
    if (result != 0) {
        fprintf(stderr, "FFmpeg command failed\n");
        return EXIT_FAILURE;
    }

    printf("Video streaming completed successfully.\n");

    return EXIT_SUCCESS;
}