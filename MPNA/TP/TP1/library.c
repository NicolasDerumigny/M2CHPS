#include <stdio.h>
#include <stdlib.h>
#include <time.h>


double prod_scal(
		const double vect1[],
		const double vect2[],
		int size) {
	double dst = 0;
	while(size--) {
		dst+=vect1[size]*vect2[size];
	}
	return dst;
}

void prod_mv(
		double dst[],
		const double matrix[],
		const double vect[],
		const int size_matrix[2]) {
	int tmp_x, tmp_y = size_matrix[1];
	while(tmp_y--) {
		tmp_x = size_matrix[0];
		dst[tmp_y]=0;
		while(tmp_x--) {
			dst[tmp_y]+=matrix[size_matrix[0]*tmp_y+tmp_x]*vect[tmp_x];
		}
	}
}

void compute_P(
		double dst[],
		const double matrix[],
		const double vect[],
		const int size_matrix[2]) {
	double * Ax = malloc(size_matrix[1]*sizeof(double));
	prod_mv(Ax, matrix, vect, size_matrix);
	prod_mv(dst, matrix, Ax, size_matrix);
	int tmp = size_matrix[1];
	while(tmp--) {
		dst[tmp]+=Ax[tmp]+vect[tmp];
	}
	free(Ax);
}

void arnoldi(
		double *dst[],
		const double matrix[],
		const double vector[],
		const int size_matrix[2],
		const int m) {
	//cf cours (q = vector, A=matrix et on stocke les q_i dans dst)
	double *w;
	w = malloc(size_matrix[1]);
	for(int k=1; k<m; ++k) {
		prod_mv(w,matrix,dst[k]);
		//
	}
	free(w);
}

int main (int argc, char *argv[], char*env[]) {
	return 0;
}