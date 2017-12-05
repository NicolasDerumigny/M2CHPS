#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda.h>

#include "libvideo.h"

#define SEUIL 50

__global__ void kernel_sobel(char * frame, int width, int length)
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

	OpenReadAndWriteVideo("./Wildlife.wmv", "./my_copy.wmv");


	printf("----------------------------------------\n");
	frame_count = getFrameCount();
	width = getWidth();
	height = getHeight();
	printf("Frame count = %d\n", frame_count); fflush(stdout);

	printf("Width  of frames: %d\n", width); fflush(stdout);
	printf("Height of frames: %d\n", height); fflush(stdout);


//	char * frames = (char *) malloc( sizeof(char) * frame_count * width * height * 3);
	char * frame1 = (char *) malloc( sizeof(char) * width * height * 3);

	char * sobel;
	sobel = (char *)malloc(sizeof(char) * width*height*3);

	int line_size = width*3;

	for(cpt_frame = 0; cpt_frame < 500 && cpt_frame < frame_count; cpt_frame ++)
	{

		printf("%d - Read frame with index\n", cpt_frame); fflush(stdout);
		readFrame_with_index(frame1, cpt_frame);


		if(cpt_frame > 200 && cpt_frame < 400)
		{
			printf("%d - GREY\n", cpt_frame); fflush(stdout);


			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//					pixel = frame->at<Vec3b>(i, j);	

					int red   = frame1[i*line_size+3*j+0];
					int green = frame1[i*line_size+3*j+1];
					int blue  = frame1[i*line_size+3*j+2];

					int moy = red/4 + (green*3/4);

					frame1[i*line_size+3*j+0] = (char)moy;
					frame1[i*line_size+3*j+1] = (char)moy;
					frame1[i*line_size+3*j+2] = (char)moy;


				}
			}

		}
		if(cpt_frame >= 400 && cpt_frame < 800)
		{
			printf("%d - SOBEL\n", cpt_frame); fflush(stdout); 


			/***************************************/
			/****************** SOBEL **************/
			/***************************************/

            /* A COMPLETER */


		}


//		printf("%d - Write frame\n", cpt); fflush(stdout);
		writeFrame (frame1);
//		printf("%d - Frame written\n", cpt); fflush(stdout);

	}
	printf("ECRITURE VIDEO FINIE\n");

	free(frame1);
	free(sobel);



	return 0;

}
