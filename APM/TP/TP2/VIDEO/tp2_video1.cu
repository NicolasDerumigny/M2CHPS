#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda.h>

#include "libvideo.h"

#define SEUIL 50

__global__ void kernel_grey(char * frame, int width, int length)
{
/* A COMPLETER */
}



int main (int argc, char * argv[])
{
	int i, j, k;
	int cpt_frame;
	int cpt = 0;
	int frame_count;
	int width, height;



	printf("Opening videos - read and write\n"); fflush(stdout);

	OpenReadAndWriteVideo("./Wildlife.wmv", "./my_copy2.wmv");


	printf("----------------------------------------\n");
	frame_count = getFrameCount();
	width = getWidth();
	height = getHeight();
	printf("Frame count = %d\n", frame_count); fflush(stdout);

	printf("Width  of frames: %d\n", width); fflush(stdout);
	printf("Height of frames: %d\n", height); fflush(stdout);


//	char * frames = (char *) malloc( sizeof(char) * frame_count * width * height * 3);
	char * frame1 = (char *) malloc( sizeof(char) * width * height * 3);

	int line_size = width*3;

	for(cpt_frame = 0; cpt_frame < 500 && cpt_frame < frame_count; cpt_frame ++)
	{

		printf("%d - Read frame with index\n", cpt_frame); fflush(stdout);
		readFrame_with_index(frame1, cpt_frame);


		if(cpt_frame > 200 && cpt_frame < 400)
		{
			printf("%d - GREY\n", cpt_frame); fflush(stdout);

        /* COLOR -> GREY */
        /* A COMPLETER */


		}
		writeFrame (frame1);

	}
	printf("ECRITURE VIDEO FINIE\n");

	free(frame1);
	free(sobel);



	return 0;

}
