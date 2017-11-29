#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include <mpi.h>
#define M 80


clock_t mv_time = 0, p_scal_time = 0;

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
	double norm = prod_scal(vector, vector,size_matrix[1]);
	norm = sqrt(norm);
	for (int i = 0; i < size_matrix[1]; ++i) {
		dst[0][i]=vector[i]/norm;
	}

	//cf cours (q = vector, A=matrix et on stocke les q_i dans dst)
	double *w, h;
	w = malloc(size_matrix[0]*sizeof(double));
	for(int k=1; k<m; ++k) {
		clock_t start, end;
		start = clock();
		prod_mv(w,matrix,dst[k],size_matrix);
		end = clock();
		mv_time += (end-start);
		for (int j=1; j<k; ++j) {
			start = clock();
			h = prod_scal(w,vector,size_matrix[0]);
			end = clock();
			p_scal_time += (end-start);
			for (int i = 0; i < size_matrix[1]; ++i) {
				w[i] = w[i] - h*dst[j][i];
			}
		}
		start = clock();
		h = sqrt(prod_scal(w,w,size_matrix[0]));
		end = clock();
		p_scal_time += (end-start);
		for(int i=0; i<size_matrix[1];++i) {
			dst[k+1][i] = w[i]/h;
		}
	}
	free(w);
}

int main (int argc, char *argv[], char*env[]) {
	//MPI_BEGIN();
	clock_t start, end;
	int size[2] = {atoi(argv[1]),atoi(argv[1])};
	double **dst, *matrix, *vector;
	dst = malloc(sizeof(double*)*(M+1));
	for (int i = 0; i<M+1; i++) {
		dst[i] = malloc(sizeof(double)*size[0]);
	}
	matrix = malloc(size[0]*size[1]*sizeof(double));
	vector = malloc(size[0]*sizeof(double));
	start = clock();
	arnoldi(dst, matrix, vector, size, M);
	end = clock();
	printf("%f;%f;%f\n",(double) (end-start)/CLOCKS_PER_SEC, (double) (mv_time)/CLOCKS_PER_SEC, (double) (p_scal_time)/CLOCKS_PER_SEC);
	free(vector);
	for (int i = 0; i<M+1; i++) {
		free(dst[i]);
	}
	free(dst);
	free(matrix);

	return 0;
}