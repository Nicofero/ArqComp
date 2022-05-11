#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <unistd.h>

#define block_size 5

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

 int compare (const void * a,const void * b){
  return ( *(double*)a - *(double*)b );
}

void free_2p(double **A,int fil,int col){
    int i,j;
    for(i=0;i<fil;i++){
        free(A[i]);
    }
    free(A);
}

int main(int argc, char *argv[]){

    double **a, **b, *c,**d,*e,f,ck[10]; // Matrices y vector de entrada que almacenan valores aleatorios
    int *ind, i, j,k, l,ii,jj;          // Vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan
    int N;                      //Tamaño de la matriz
    FILE *p;

    if( argc != 3 ){
      printf("Número de argumentos incorrecto\n");
      exit(EXIT_FAILURE);
    }

    if ((p=fopen(argv[2],"a+"))==NULL){
        perror("ERROR: ");
        exit (-1);
    }    

    N = atoi(argv[1]);

    srand(0);

    a = (double **)malloc(N * sizeof(double));
    for (i = 0; i < N; i++){
        a[i]=(double *) malloc(8*sizeof(double));
    }
    
    b = (double **)malloc(8 * sizeof(double));
    for (i = 0; i < 8; i++){
        b[i]=(double *) malloc(N*sizeof(double));
    }
    c = (double *)malloc(8 * sizeof(double));
    d = (double **)malloc(N *  sizeof(double));
    for (i = 0; i < N; i++){
        d[i]=(double *) malloc(N*sizeof(double));
    }
    e = (double *)malloc(N * sizeof(double));
    ind = (int *)malloc(N * sizeof(int));

    // Inicialización de a
    for (i = 0; i < N; i++){
        for (int j = 0; j < 8; j++){
            a[i][j] = (double)rand() / RAND_MAX;
        }
        ind[i] = i; //Optimizacion inicializacion
    }

    // Inicialización de b
    for (i = 0; i < 8; i++){
        for (j = 0; j < N; j++){
            b[i][j] = (double)rand() / RAND_MAX;
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
    for(l=0;l<10;l++){
        start_counter();

    
        for (i=0; i< N; i+=block_size) {
            for (j=0; j< N; j+=block_size) {
                for (ii=i; ii<i+block_size; ii++) {
                    for (jj=j; jj<j+block_size; jj++) {
                        d[ii][jj]=0;
                        d[ii][jj] += 2*a[ii][0]*(b[0][jj]-c[0]);
                        d[ii][jj] += 2*a[ii][(1)]*(b[(1)][jj]-c[(1)]);
                        d[ii][jj] += 2*a[ii][(2)]*(b[(2)][jj]-c[(2)]);
                        d[ii][jj] += 2*a[ii][(3)]*(b[(3)][jj]-c[(3)]);
                        d[ii][jj] += 2*a[ii][(4)]*(b[(4)][jj]-c[(4)]);
                        d[ii][jj] += 2*a[ii][(5)]*(b[(5)][jj]-c[(5)]);
                        d[ii][jj] += 2*a[ii][(6)]*(b[(6)][jj]-c[(6)]);
                        d[ii][jj] += 2*a[ii][(7)]*(b[(7)][jj]-c[(7)]);
                    }
                }
            }
        }

        for (i = 0,f=0; i < N; i+=5){
            e[i] = d[ind[i]][ind[i]] / 2;
            e[i+1] = d[ind[i+1]][ind[i+1]] / 2;
            e[i+2] = d[ind[i+2]][ind[i+2]] / 2;
            e[i+3] = d[ind[i+3]][ind[i+3]] / 2;
            e[i+4] = d[ind[i+4]][ind[i+4]] / 2;
            f += e[i] + e[i+1] + e[i+2] + e[i+3] + e[i+4];
        }

        ck[l]=get_counter();
    }

    qsort(ck,10,sizeof(double),compare);    //Ordenamos el vector de tiempos

    printf("f=%lf\n", f);
    printf("\nClocks=%1.10lf \n",((ck[4]+ck[5])/2));    //Imprimimos la mediana de los tiempos

    if(N!=3000)   fprintf(p,"%lf,",((ck[4]+ck[5])/2));  //Impresion archivo .r
    else    fprintf(p,"%lf)\n",((ck[4]+ck[5])/2));

    //Liberacion de memoria
    free_2p(a,N,8);
    free_2p(b,8,N);
    free(c);
    free_2p(d,N,N);
    free(e);
}