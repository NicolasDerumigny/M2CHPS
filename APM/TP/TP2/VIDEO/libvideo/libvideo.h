#include <opencv2/core/highgui_c.h>
#include <opencv2/core/core_c.h>

#include <stdio.h>
#include <stdlib.h>


int getFrameCount();
int getWidth();
int getHeight();

int OpenReadAndWriteVideo(const char * read_name, const char * write_name);
int readFrame(char * tab);
int readFrame_with_index(char * tab, int idx);
int readAllFrames(char * tab);
int writeFrame(char * tab);

int closeVideos();

