#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

void main(){

    double *A,*b,*res,*aux;
    __m256d c,d,r,s; 
    int i,j;

    A=(double*)_mm_malloc(64*sizeof(double),64);
    b=(double*)aligned_alloc(32,64*sizeof(double));
    aux=(double*)aligned_alloc(32,4*sizeof(double));

    srand(1);

    for(i=0;i<64;i++){
        A[i]=(double)rand()/RAND_MAX;
        b[i]=(double)rand()/RAND_MAX;
    }

    printf("%p,%p\n",A,b);

    for(i=0;i<64;i+=4){
        
        for(j=0;j<4;j++)    aux[j]=b[i+j];
        c = _mm256_load_pd(aux);
        r= _mm256_add_pd(c,c);
        res = &r;
        for(j=0;j<4;j++)printf("%lf ",res[j]);
        printf("\n");
    }
    
}