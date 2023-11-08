#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

int main() {
    Matriz A, B, C;

    
    A = leMatriz("matriz_A.txt");
    B = leMatriz("matriz_B.txt");

  
    printf("Matriz A:\n");
    imprimeMatriz(A);
    

    
    C = somaMatrizes(A, B);
    printf("\nSoma de A e B:\n");
    
    imprimeMatriz(C);

    
    C = multiplicaMatrizes(A, B);
    printf("\nMultiplicacao de A e B:\n");
     printf("PASSEI POR AQ\n"); 
    imprimeMatriz(C);

 
    return 0;
}
