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

 //Funcion para utilizar el qsort, que ordena los resultados obtenidos, para posteriormente calcular la mediana
 int compare (const void * a,const void * b){
  return ( *(double*)a - *(double*)b );
}

int main(int argc,char* argv[]){

  //Definicion de variables necesarias, algunas son constantes como L, sin embargo, como se debe modificar en cada una de las iteraciones, lo almacenamos aqui, en vez de como una constante.
  double ck[10],*A,sum[10]={0,0,0,0,0,0,0,0,0,0};
  int R=0,L[]={0.5*S1, 1.5*S1,  0.5*S2,  0.75*S2,  2*S2, 4*S2,  8*S2},i,*ind,j,k;
  FILE *p;    //File para almacenar el fichero R

  printf("%s\n",argv[2]);

  //Abrimos el archivo en el cual se escribiran los resultados en formato que pueda utilizar R (en caso de que no exista, te lo crea)
  if ((p=fopen(argv[2],"a+"))==NULL){
    perror("ERROR: ");
    exit (-1);
  }

  //Comprobamos que el valor de D se haya pasado por linea de comandos
  if (argc!=3){
    fprintf(stderr,"ERROR: Introduce el parámetro D en linea de comandos\n");
    exit(-1);
  }

  D=atoi(argv[1]);  //Pasamos el argumento a un valor de D (int)

  fprintf(p,"C_%d = c(",D); //Impresion del inicio de los resultados para tratar con el programa R

  for(i=0;i<7;i++){
    if((R=getR(L[i]))<0){  //Obtenemos el valor de R, usando la funcion posterior
      fprintf(stderr,"ERROR: en el calculo de R");
      exit(-1);
    }
    ind = (int*) malloc(R*sizeof(int));   //Creación del array de indices, de tamaño tantos indices como queremos tomar
    A=(double*)_mm_malloc(R*D*sizeof(double),64);   //El tamaño de linea de las caches es siempre de 64 Bytes

    for(j=0;j<R;j++)  ind[j]=j*D;   //Array de indicadores

    for(j=0;j<(R*D);j++)  A[j]=(double)rand()/RAND_MAX;   //Rellenamos el vector A con elementos aleatorios
    printf("ITERACION: %d\n\nEl tamaño de R es: %d\n",i,R);
    printf("El valor de L es: %d\n",L[i]);

    for (k=0;k<10;k++,sum[k]=0){  //Bucle para realizar las 10 distintas iteraciones de la suma
      start_counter();  //Inicio counter

      for (j=0;j<R;j++) sum[k]+=A[ind[j]];   //Suma de los R elementos dados

      ck[k]=get_counter();  //Fin de counter y lo recogemos en el array de doubles

      printf("\n Clocks=%1.10lf \n",ck[k]); //Impresion, que ayuda a evitar optimizaciones
      printf("La suma da: %lf\n",sum[k]);
    }

    qsort(ck,10,sizeof(double),compare);    //Ordenamos el array de tiempos para poder calcular la mediana posteriormente usando la funcion qsort (estrategia quicksort)

    for(k=0;k<10;k++) printf("%lf < ",ck[k]);   //Impresion de los valores de tiempo ordenados

    printf ("El tiempo medio de ciclos de reloj (mediana) es: %lf (Por unidad: %lf)\n\n___________________________\n",(ck[4]+ck[5])/2,((ck[4]+ck[5])/2)/R); //Impresion mediana

    if(i<6)   fprintf(p,"%lf,",((ck[4]+ck[5])/2)/R);  //Impresion archivo .r
    else    fprintf(p,"%lf)\n",((ck[4]+ck[5])/2)/R);

    _mm_free(A);  //Liberamos memoria del vector A e ind (dinamicos)
    free(ind);
  }

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas start_counter/get_counter */
  mhz(1,1);
  fclose(p);
  return 0;
}

//AL IGUAL ESTO ESTA MAL, MIRARLO
int getR(int L){
  int R=-1;
  if(D<8) R=(int)round((8./(float)D)*L);
  else R=L;
  return R;
}
