#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <unistd.h>

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

int main(int argc, char *argv[]){

    double **a, **b, *c,**d,*e,f,ck[10]; // Matrices y vector de entrada que almacenan valores aleatorios
    int *ind, i, j, k,l;          // Vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan
    int N;                      //Tamaño de la matriz
    FILE* p;

    //Comprobamos que el valor de N se haya pasado por linea de comandos
    if (argc!=3){
        fprintf(stderr,"ERROR: Introduce el parámetro D en linea de comandos\n");
        exit(-1);
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
        for (j = 0; j < 8; j++){
            a[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Inicialización de b
    for (i = 0; i < 8; i++){
        for (j = 0; j < N; j++){
            b[i][j] = (double)rand() / RAND_MAX;
        }
        c[i] = (double)rand() / RAND_MAX;
    }

    // Inicialización de c
    /*for (i = 0; i < 8; i++){
        c[i] = (double)rand() / RAND_MAX;
    }*/

    // Inicialización del vector de índices
    for (i = 0; i < N; i++)
    {
        ind[i] = i;  
    }

    //Aleatorizacion del vector de indices
    for(i=0;i<N;i++){
        k = rand()%N;
        j = ind[i];

        ind[i]=ind[k];
        ind[k] = j;
    }

    start_counter();

    printf("\n\nN=%d\n",N);
    for(l=0;l<10;l++){
        // Inicialización de d
        for (i = 0; i < N; i++){ //filas
            for (j = 0; j < N; j++){ // columnas
                d[i][j] = 0;
            }
        }

        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                for (k = 0; k < 8; k++){
                    d[i][j] += 2*a[i][k]*(b[k][j]-c[k]);
                }
            }
        }

        
        for (i = 0,f=0; i < N; i++){
            e[i] = d[ind[i]][ind[i]] / 2;
            f += e[i];
        }

        printf("f=%lf\n", f);

        ck[l]=get_counter();

        printf("\nClocks=%1.10lf \n",ck[l]);
    }
    qsort(ck,10,sizeof(double),compare);

    if(N!=3000)   fprintf(p,"%lf,",((ck[4]+ck[5])/2));  //Impresion archivo .r
    else    fprintf(p,"%lf)\n",((ck[4]+ck[5])/2));
}