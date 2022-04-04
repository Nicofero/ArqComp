#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <unistd.h>

#define N 100

void start_counter();
double get_counter();

/* Initialize the cycle counter */


 static unsigned cyc_hi = 0;
 static unsigned cyc_lo = 0;


 /* Set *hi and *lo to the high and low order bits of the cycle counter.
 Implementation requires assembly code to use the rdtsc instruction. */
 void access_counter(unsigned *hi, unsigned *lo)
 {
 asm("rdtsc; movl %%edx,%0; movl %%eax,%1" /* Read cycle counter */
 : "=r" (*hi), "=r" (*lo) /* and move results to */
 : /* No input */ /* the two outputs */
 : "%edx", "%eax");
 }

 /* Record the current value of the cycle counter. */
 void start_counter(){
 access_counter(&cyc_hi, &cyc_lo);
 }

 /* Return the number of cycles since the last call to start_counter. */
 double get_counter(){
 unsigned ncyc_hi, ncyc_lo;
 unsigned hi, lo, borrow;
 double result;

 /* Get cycle counter */
 access_counter(&ncyc_hi, &ncyc_lo);

 /* Do double precision subtraction */
 lo = ncyc_lo - cyc_lo;
 borrow = lo > ncyc_lo;
 hi = ncyc_hi - cyc_hi - borrow;
 result = (double) hi * (1 << 30) * 4 + lo;
 if (result < 0) {
 fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
 }
 return result;
 }

int main(){

    double *a, *b, *c,*d,*e,f,ck; // Matrices y vector de entrada que almacenan valores aleatorios
    int *ind, i, j, k;          // Vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan

    srand(0);

    a = (double *)malloc(N * 8 * sizeof(double));
    b = (double *)malloc(N * 8 * sizeof(double));
    c = (double *)malloc(8 * sizeof(double));
    d = (double *)malloc(N * N * sizeof(double));
    e = (double *)malloc(N * sizeof(double));
    ind = (int *)malloc(N * sizeof(int));

    // Inicialización de a
    for (i = 0; i < N; i++){
        for (int j = 0; j < 8; j++){
            a[i*N + j] = (double)rand() / RAND_MAX;
        }
        ind[i] = i; //Optimizacion inicializacion
    }

    // Inicialización de b
    for (i = 0; i < 8; i++){
        for (j = 0; j < N; j++){
            b[i*8 + j] = (double)rand() / RAND_MAX;
        }
        c[i] = (double)rand() / RAND_MAX; //Optimizacion inicializacion
    }

    //Aleatorizacion del vector de indices
    for(i=0;i<N;i++){
        k = rand()%N;
        j = ind[i];

        ind[i]=ind[k];
        ind[k] = j;
    }

    start_counter();

    // Inicialización de d
    /*
    for (i = 0; i < N; i++){ //filas
        for (j = 0; j < N; j++){ // columnas
            d[i*N + j] = 0;
        }
    }*/

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            d[i*N + j] = 0;     //Inicializacion de d
            for (k = 0; k < 8; k++){
                d[i*N + j] += 2*a[i*N + k]*(b[k*8 + j]-c[k]);
            }
        }
    }

    for (i = 0,f=0; i < N; i+=2){
        e[i] = d[ind[i]*N + ind[i]] / 2;
        e[i+1] = d[ind[i+1]*N + ind[i+1]] / 2;
        f += e[i] + e[i+1];
    }

    printf("f=%lf\n", f);

    ck=get_counter();

    printf("\nClocks=%1.10lf \n",ck);
}