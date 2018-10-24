#include <stdio.h>
#include <gsl/gsl_blas.h>
#include <sys/time.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector_double.h>
#include <zconf.h>

double rand_nonminus_double(int int_min, int int_max, int max_precision){
    srand((unsigned int) time(NULL));
    int bigInt = rand() % (int)pow(10,max_precision);
    int smallInt = rand() % int_max + int_min;
    return ((double) bigInt) / pow(10,(int)log10(bigInt)) + (double) smallInt;
}


double * create_matrix(int rows, int columns, int isZeroedMatrix){

    double * array = calloc((size_t) rows * columns, sizeof(double));

    if(isZeroedMatrix)
        return array;

    for(int i = 0; i < rows*columns; i++)
        array[i] = rand_nonminus_double(1,100,6);


    return array;
}

double calc_time(clock_t start, clock_t stop){
    return (double) (stop - start);
}


int main () {
    clock_t start_time;
    clock_t stop_time;
    int size;

    // results[i][0] - algo, results[i][1] - size, results[i][2] - time
    int results[140][3];

    for(int i = 0; i < 7; i++){

        /* Matrixes algo 0 */
        for(int j = 0; j < 10; j++){
            size = (int) pow(4,i+3);
            double * array_A = create_matrix(1,size,0);
            double * array_B = create_matrix(size,1,0);
            double * array_C = create_matrix(1,1,1);

            gsl_matrix_view matrix_A = gsl_matrix_view_array(array_A, 1, size);
            gsl_matrix_view matrix_B = gsl_matrix_view_array(array_B, size, 1);
            gsl_matrix_view matrix_C = gsl_matrix_view_array(array_C, 1, 1);

            start_time = clock();
            gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
                            1.0, &matrix_A.matrix, &matrix_B.matrix,
                            0.0, &matrix_C.matrix);
            stop_time = clock();

            results[j + i*10][0] = 0;
            results[j + i*10][1] = size;
            results[j + i*10][2] = (int)(calc_time(start_time,stop_time));

            //printf("%f\n", array_C[0]);

            free(array_A);
            free(array_B);
            free(array_C);
        }

        /* Vectors algo 1 */
        for(int j = 0; j < 10; j++){
            size = (int) pow(4,i+3);
            double * array_A = create_matrix(1,size,0);
            double * array_B = create_matrix(size,1,0);
            double * array_C = create_matrix(1,1,1);

            gsl_vector_view vector_A = gsl_vector_view_array(array_A, size);
            gsl_vector_view vector_B = gsl_vector_view_array(array_B, size);
            gsl_vector_view vector_C = gsl_vector_view_array(array_C, 1);

            start_time = clock();
            gsl_blas_ddot (&vector_A.vector, &vector_B.vector, array_C);
            stop_time = clock();

            results[j + i*10 + 70][0] = 1;
            results[j + i*10 + 70][1] = size;
            results[j + i*10 + 70][2] = (int)(calc_time(start_time,stop_time));

            //printf("%f\n", array_C[0]);

            free(array_A);
            free(array_B);
            free(array_C);
        }

    }

    for(int i = 0; i < 140; i++){
        printf("%d\t%d\t%d\n", results[i][0], results[i][1], results[i][2]);
    }

    FILE * fp = fopen("result.csv", "w+");
    fprintf(fp,"algorithm, size, cpu_time\n");
    for(int i = 0; i < 140; i++){
        fprintf(fp,"%s, %d, %d\n", results[i][0] == 0 ? "gsl_blas_dgemm" : "gsl_blas_ddot", results[i][1], results[i][2]);
    }
    fclose(fp);

    return 0;
}