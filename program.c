#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <unistd.h>
#include <math.h>
#define S1 512
#define S2 4096
int D;

void start_counter();
double get_counter();
double mhz();
int getR(int L);


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
 void start_counter()
 {
 access_counter(&cyc_hi, &cyc_lo);
 }

 /* Return the number of cycles since the last call to start_counter. */
 double get_counter()
 {
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

double mhz(int verbose, int sleeptime)
 {
 double rate;

 start_counter();
 sleep(sleeptime);
 rate = get_counter() / (1e6*sleeptime);
 if (verbose)
 printf("\n Processor clock rate = %.1f MHz\n", rate);
 return rate;
 }




int main(int argc,char* argv[]){

  double ck,*A,sum=0;
  int R=0,L[]={0.5*S1, 1.5*S1,  0.5*S2,  0.75*S2,  2*S2, 4*S2,  8*S2},i,*ind,j;

  //Comprobamos que el valor de D se haya pasado por linea de comandos
  if (argc!=2){
    fprintf(stderr,"ERROR: Introduce el parámetro D en linea de comandos\n");
    exit(-1);
  }

  D=atoi(argv[1]);  //Pasamos el argumento a un valor de D (int)

  for(i=0;i<7;i++){
    R=getR(L[i]);
    ind = (int*) malloc(R*sizeof(int));
    A=(double*)_mm_malloc(R*D*sizeof(double),64);   //El tamaño de linea de las caches es siempre de 64 Bytes

    for(j=0;j<R;j++)  ind[j]=j*D;   //Array de indicadores

    for(j=0;j<(R*D);j++)  A[j]=(double)rand()/RAND_MAX;   //Rellenamos el vector A con elementos aleatorios
    printf("El tamaño de R es: %d",R);
    start_counter();

    for (j=0;j<R;j++) sum+=A[ind[j]];   //Suma de los R elementos dados

    ck=get_counter();

    printf("\n Clocks=%1.10lf \n",ck);
    printf("La suma da: %lf\n",sum);
    _mm_free(A);
    free(ind);
  }

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas start_counter/get_counter */
  mhz(1,1);



  return 0;
}

int selectL(int i){
  int l;

}

//TODO
//AL IGUAL ESTO ESTA MAL, MIRARLO
int getR(int L){
  int R=-1;
  printf("%lf\n",round(8/(float)D));
  if (D<8)  R=(int)round(8/(float)D)*L;
  else R=L;
  return R;
}
