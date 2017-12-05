#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libvideo.h"

#define SEUIL 50


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


			for(i=1; i<height-1; i++)
			{
				for(j=1; j<width-1; j++)
				{


					int pixel_blue_no, pixel_blue_n, pixel_blue_ne; 				
					int pixel_blue_so, pixel_blue_s, pixel_blue_se;
					int pixel_blue_o , pixel_blue  , pixel_blue_e ;

					int pixel_green_no, pixel_green_n, pixel_green_ne; 				
					int pixel_green_so, pixel_green_s, pixel_green_se;
					int pixel_green_o , pixel_green  , pixel_green_e ;


					int pixel_red_no, pixel_red_n, pixel_red_ne; 				
					int pixel_red_so, pixel_red_s, pixel_red_se;
					int pixel_red_o , pixel_red  , pixel_red_e ;


					float deltaX_red, deltaX_blue, deltaX_green;
					float deltaY_red, deltaY_blue, deltaY_green;
					float val_red, val_green, val_blue;

					pixel_blue_no = frame1[(i+1)*line_size+3*(j-1)+0];
					pixel_blue_n  = frame1[(i+1)*line_size+3*(j  )+0];
					pixel_blue_ne = frame1[(i+1)*line_size+3*(j+1)+0];
					pixel_blue_so = frame1[(i+1)*line_size+3*(j-1)+0];
					pixel_blue_s  = frame1[(i+1)*line_size+3*(j  )+0];
					pixel_blue_se = frame1[(i+1)*line_size+3*(j+1)+0];
					pixel_blue_o  = frame1[(i  )*line_size+3*(j-1)+0];
					pixel_blue_e  = frame1[(i  )*line_size+3*(j+1)+0];

					pixel_green_no = frame1[(i+1)*line_size+3*(j-1)+1];
					pixel_green_n  = frame1[(i+1)*line_size+3*(j  )+1];
					pixel_green_ne = frame1[(i+1)*line_size+3*(j+1)+1];
					pixel_green_so = frame1[(i+1)*line_size+3*(j-1)+1];
					pixel_green_s  = frame1[(i+1)*line_size+3*(j  )+1];
					pixel_green_se = frame1[(i+1)*line_size+3*(j+1)+1];
					pixel_green_o  = frame1[(i  )*line_size+3*(j-1)+1];
					pixel_green_e  = frame1[(i  )*line_size+3*(j+1)+1];

					pixel_red_no  = frame1[(i+1)*line_size+3*(j-1)+2];
					pixel_red_n   = frame1[(i+1)*line_size+3*(j  )+2];
					pixel_red_ne  = frame1[(i+1)*line_size+3*(j+1)+2];
					pixel_red_so  = frame1[(i+1)*line_size+3*(j-1)+2];
					pixel_red_s   = frame1[(i+1)*line_size+3*(j  )+2];
					pixel_red_se  = frame1[(i+1)*line_size+3*(j+1)+2];
					pixel_red_o   = frame1[(i  )*line_size+3*(j-1)+2];
					pixel_red_e   = frame1[(i  )*line_size+3*(j+1)+2];



					deltaX_red = -pixel_red_no + pixel_red_ne - 2*pixel_red_o + 2*pixel_red_e - pixel_red_so + pixel_red_se;                
					deltaX_green = -pixel_green_no + pixel_green_ne - 2*pixel_green_o + 2*pixel_green_e - pixel_green_so + pixel_green_se;            
					deltaX_blue = -pixel_blue_no + pixel_blue_ne - 2*pixel_blue_o + 2*pixel_blue_e - pixel_blue_so + pixel_blue_se;             

					deltaY_red = pixel_red_se + 2*pixel_red_s + pixel_red_so - pixel_red_ne - 2*pixel_red_n - pixel_red_no;
					deltaY_green = pixel_green_se + 2*pixel_green_s + pixel_green_so - pixel_green_ne - 2*pixel_green_n - pixel_green_no;
					deltaY_blue = pixel_blue_se + 2*pixel_blue_s + pixel_blue_so - pixel_blue_ne - 2*pixel_blue_n - pixel_blue_no;


					val_red = sqrt(deltaX_red * deltaX_red + deltaY_red * deltaY_red)/4;
					val_green = sqrt(deltaX_green * deltaX_green + deltaY_green * deltaY_green)/4;
					val_blue = sqrt(deltaX_blue * deltaX_blue + deltaY_blue * deltaY_blue)/4;



					if((val_red + val_green + val_blue)/3 > SEUIL)
					{
						sobel[i*(width*3)+3*j+2] = 255;
						sobel[i*(width*3)+3*j+1] = 255;
						sobel[i*(width*3)+3*j+0] = 255;
					}
					else
					{
						sobel[i*(width*3)+3*j+2] = 0;
						sobel[i*(width*3)+3*j+1] = 0;
						sobel[i*(width*3)+3*j+0] = 0;
					}
				}
			}

			for(i=1; i<height-1; i++)
			{
				for(j=1; j<width-1; j++)
				{

					frame1[i*line_size+3*j+2] = sobel[i*(width*3)+3*j+2];
					frame1[i*line_size+3*j+1] = sobel[i*(width*3)+3*j+1];
					frame1[i*line_size+3*j+0] = sobel[i*(width*3)+3*j+0];

				}
			}

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
