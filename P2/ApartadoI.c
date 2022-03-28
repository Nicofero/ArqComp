#include <stdio.h>
#include <stdlib.h>

#define N 10
int main()
{

    double a[N][8], b[8][N], c[8]; // Matrices y vector de entrada que almacenan vectores aleatorios
    int ind[N];                    // Vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan

    //Inicialización de a, b,c
    for (int i = 0; i < N; i++)
    {
        
    }
    

    double f;
    double e[N];
    double d[N][N];

    //Inicialización de d
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            d[N][N] = 0;
        }
    }

    

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                d[i][j] += 2 * a[i][k] * (b[k][j] - c[k]);
            }
        }
    }
    f = 0;
    for (int i = 0; i < N; i++)
    {
        e[i] = d[ind[i]][ind[i]] / 2;
        f += e[i];
    }


    printf("f=%lf\n", f);
}