#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>


double subtract_time(struct timeval a, struct timeval b)
{
    double tmp_a = ((double) a.tv_sec)  + (((double) a.tv_usec) / 1000000);
    double tmp_b = ((double) b.tv_sec)  + (((double) b.tv_usec) / 1000000);
    return tmp_a - tmp_b;
}

void naive_multiplication(double **matrixA, double **matrixB, double **result, int size) {

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            for(int k = 0; k < size; k++)
                result[i][j] += matrixA[i][k] * matrixB[k][j];
}

void better_multiplication(double **matrixA, double **matrixB, double **result, int size) {

    for(int i = 0; i < size; i++)
        for(int k = 0; k < size; k++)
            for(int j = 0; j < size; j++)
                result[i][j] += matrixA[i][k] * matrixB[k][j];
}

double ** create_empty_matrix(int rows, int columns){
    double **matrix = calloc(rows, sizeof(double*));
    for(int i = 0; i < rows; i++)
        matrix[i] = calloc(columns, sizeof(double));

    return matrix;
}

double rand_nonminus_double(int int_min, int int_max, int max_precision){
    srand((unsigned int) time(NULL));
    int bigInt = rand() % (int)pow(10,max_precision);
    int smallInt = rand() % int_max + int_min;
    return ((double) bigInt) / pow(10,(int)log10(bigInt)) + (double) smallInt;
}

double ** randomly_filled_matrix(double **matrix, int rows, int columns){

    for(int i_rows = 0; i_rows < rows; i_rows++){
        for(int i_columns = 0; i_columns < columns; i_columns++){
            matrix[i_rows][i_columns] = rand_nonminus_double(0,100,5);
        }
    }
    return matrix;
}


void free_matrix(double ** matrix, int rows, int columns){
    for(int i_rows = 0; i_rows < rows; i_rows++){
            free(matrix[i_rows]);
    }
    free(matrix);
}

gsl_matrix* get_random_gsl_matrix(int height, int width) {

    gsl_matrix *matrix = gsl_matrix_alloc(height, width);

    for(int i = 0; i < height * width; i++) {
        double value = rand_nonminus_double(0,100,5);
        gsl_matrix_set(matrix, i / width, i % width, value);
    }

    return matrix;
}

void write_to_file(FILE * file, char* alg_name,int size, double time){
    fprintf(file,"%s, %d, %lf\n", alg_name, size, time);
}

int main() {
    FILE * fp = fopen("c_result.csv", "w+");
    struct timeval time_start, time_stop;


    /* naive multiplication */
    for(int i = 0; i < 3; i++){
        for(int j = 3; j < 10; j++){
            int size = 200 * j;
            double ** matrix_A = randomly_filled_matrix(create_empty_matrix(size, size), size, size);
            double ** matrix_B = randomly_filled_matrix(create_empty_matrix(size, size), size, size);
            double ** result = create_empty_matrix(size, size);

            gettimeofday(&time_start, NULL);
            naive_multiplication(matrix_A, matrix_B, result, size);
            gettimeofday(&time_stop, NULL);

            write_to_file(fp, "c_naive_multiplication", size, subtract_time(time_stop, time_start));


            free_matrix(matrix_A, size, size);
            free_matrix(matrix_B, size, size);
            free_matrix(result, size, size);
        }
    }

    /* better multiplication */
    for(int i = 0; i < 3; i++){
        for(int j = 3; j < 10; j++){
            int size = 200 * j;
            double ** matrix_A = randomly_filled_matrix(create_empty_matrix(size, size), size, size);
            double ** matrix_B = randomly_filled_matrix(create_empty_matrix(size, size), size, size);
            double ** result = create_empty_matrix(size, size);

            gettimeofday(&time_start, NULL);
            better_multiplication(matrix_A, matrix_B, result, size);
            gettimeofday(&time_stop, NULL);

            write_to_file(fp, "c_better_multiplication", size, subtract_time(time_stop, time_start));


            free_matrix(matrix_A, size, size);
            free_matrix(matrix_B, size, size);
            free_matrix(result, size, size);
        }
    }

    /* GSL_BLAS multiplication */
    for(int i = 0; i < 3; i++){
        for(int j = 3; j < 10; j++){
            int size = 200 * j;
            gsl_matrix *matrix_A = get_random_gsl_matrix(size, size);
            gsl_matrix *matrix_B = get_random_gsl_matrix(size, size);
            gsl_matrix *result = gsl_matrix_alloc(size, size);

            gettimeofday(&time_start, NULL);
            gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, matrix_A, matrix_B, 0.0, result);
            gettimeofday(&time_stop, NULL);

            write_to_file(fp, "c_GSL_multiplication", size, subtract_time(time_stop, time_start));


            gsl_matrix_free(result);
            gsl_matrix_free(matrix_A);
            gsl_matrix_free(matrix_B);
        }
    }

    return 0;
}