#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	printf("Enter a positive number n\n");
	unsigned int n;
	scanf("%i",&n);

	double *in;
	fftw_complex *out;
    fftw_plan p;


    FILE * results = fopen("results", "w");
    
    in = (double*) malloc(sizeof(double) * n);

    for(unsigned int k=0; k<n; ++k) {
    	in[k]=sin((10*k*M_PI)/n);
    }

    for (unsigned int i=0; i<n; ++i) {
    	fprintf(results, "%f ", in[i]);
    }
    fprintf(results, "\n");

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (n/2+1));
    p = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);
    
    fftw_execute(p);

    for (unsigned int i=0; i<(n/2+1); ++i) {
    	fprintf(results, "%f ", out[i][0]);
    }

    fprintf(results, "\n");

    for (unsigned int i=0; i<(n/2+1); ++i) {
    	fprintf(results, "%f ", out[i][1]);
    }



    fftw_destroy_plan(p);
    free(in);
    fftw_free(out);
    fclose(results);

	return 0;
}