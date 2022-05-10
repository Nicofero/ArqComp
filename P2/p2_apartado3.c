#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <unistd.h>
#include <immintrin.h>

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

    double **a, **b, *c,**d,*e,f,ck[10],*twd; // Matrices y vector de entrada que almacenan valores aleatorios
    int *ind, i, j, k,l;          // Vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan
    int N;                      //Tamaño de la matriz
    __m256d aa,bb,cc,r,tw,r2;
    FILE* p;

    //Comprobamos que el valor de N se haya pasado por linea de comandos
    if (argc!=3){
        fprintf(stderr,"ERROR: Los argumentos deben introducirse en este orden: N(tamaño de los vectores y matriz) archivo_de_salida\n");
        exit(-1);
    }

    if ((p=fopen(argv[2],"a+"))==NULL){
        perror("ERROR: ");
        exit (-1);
    }    

    N = atoi(argv[1]);

    srand(0);

    a = (double **)aligned_alloc(32,N * sizeof(double));
    for (i = 0; i < N; i++){
        a[i]=(double *) aligned_alloc(32,8*sizeof(double));
    }
    
    b = (double **)aligned_alloc(32,N * sizeof(double));
    for (i = 0; i < N; i++){
        b[i]=(double *) aligned_alloc(32,8*sizeof(double));
    }
    c = (double *)aligned_alloc(32,8 * sizeof(double));
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
        ind[i] = i; //Optimizacion inicializacion
    }

    // Inicialización de b
    for (i = 0; i < 8; i++){
        for (j = 0; j < N; j++){
            b[j][i] = (double)rand() / RAND_MAX;
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

    twd = (double*)aligned_alloc(32,4*sizeof(double));
    for(i=0;i<4;i++)   twd[i]=2.0;    

    tw = _mm256_load_pd(twd);
    
    printf("N=%d\n",N);
    for(l=0;l<10;l++){
        start_counter();
        
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                d[i][j] = 0;     //Inicializacion de d
                
                //Cargamos los 4 primeros datos del bucle desenrrollado utilizado en el ejercicio anterior 
                aa = _mm256_load_pd(&a[i][0]);  
                bb = _mm256_load_pd(&b[j][0]);
                cc = _mm256_load_pd(&c[0]);
                
                //Realizamos las operaciones con los primeros 4 elementos y lo almacenamos en r
                r = _mm256_sub_pd (bb,cc);
                aa = _mm256_mul_pd(aa,tw);
                r = _mm256_mul_pd(aa,r);               

                //Cargamos los segundos 4 datos del bucle desarrollado
                aa = _mm256_load_pd(&a[i][4]);
                bb = _mm256_load_pd(&b[j][4]);
                cc = _mm256_load_pd(&c[4]);            

                //Realizamos las operaciones con los primeros 4 elementos y lo almacenamos en r2    
                r2 = _mm256_sub_pd (bb,cc);
                aa = _mm256_mul_pd(aa,tw);
                r2 = _mm256_mul_pd(aa,r2);

                r= _mm256_add_pd(r,r2); //Sumamos r y r2

                d[i][j] += r[0] + r[1] + r[2] + r[3];   //Sumamos los elementos de r lo que nos da el resultado
            }
        }
        for (i = 0,f=0; i < N; i+=5){
            //Realizar este bucle mediante procesamiento vectorial no produce ninguna mejora con respecto a este modo
            e[i] = d[ind[i]][ind[i]] / 2;
            e[i+1] = d[ind[i+1]][ind[i+1]] / 2;
            e[i+2] = d[ind[i+2]][ind[i+2]] / 2;
            e[i+3] = d[ind[i+3]][ind[i+3]] / 2;
            e[i+4] = d[ind[i+4]][ind[i+4]] / 2;
            f += e[i] + e[i+1] + e[i+2] + e[i+3] + e[i+4];
        }

        ck[l]=get_counter();
        
    }
    qsort(ck,10,sizeof(double),compare);

    printf("f=%lf\n", f);
    printf("\nClocks=%1.10lf \n",((ck[4]+ck[5])/2));

    if(N!=3000)   fprintf(p,"%lf,",((ck[4]+ck[5])/2));  //Impresion archivo .r
    else    fprintf(p,"%lf)\n",((ck[4]+ck[5])/2));

    //Liberacion de memoria
    free_2p(a,N,8);
    free_2p(b,8,N);
    free(c);
    free_2p(d,N,N);
    free(e);
    free(twd);
}