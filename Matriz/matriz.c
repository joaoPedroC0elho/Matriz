#include <stdio.h>
#include <stdlib.h>

#include "matriz.h"

Matriz criaMatriz(int nlin, int ncol) {
    Matriz matriz;
    matriz.nlin = nlin;
    matriz.ncol = ncol;
    matriz.cabeca = (Celula *)malloc(sizeof(Celula));
    matriz.cabeca->linha = -1;
    matriz.cabeca->coluna = -1;
    matriz.cabeca->direita = matriz.cabeca;
    matriz.cabeca->abaixo = matriz.cabeca;
    matriz.cabeca->valor = 0.0;

    return matriz;
}


void imprimeMatriz(Matriz A) {
    Celula *atual = A.cabeca->abaixo;

    for (int i = 0; i < A.nlin; i++) {
        Celula *temp = atual;
        for (int j = 0; j < A.ncol; j++) {
            if (temp->coluna == j) {
                printf("%.2f ", temp->valor);
                temp = temp->direita;
            } else {
                printf("0.00 ");
            }
        }
        printf("\n");
        atual = atual->abaixo;
    }
}





Matriz leMatriz(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    int nlin, ncol;
    fscanf(arquivo, "%d, %d", &nlin, &ncol);

    Matriz matriz = criaMatriz(nlin, ncol);

    while (!feof(arquivo)) {
        int linha, coluna;
        float valor;
        fscanf(arquivo, "%d, %d, %f", &linha, &coluna, &valor);

        Celula *novaCelula = (Celula *)malloc(sizeof(Celula));
        novaCelula->linha = linha;
        novaCelula->coluna = coluna;
        novaCelula->valor = valor;

        Celula *atualLinha = matriz.cabeca;
        while (atualLinha->abaixo != matriz.cabeca && atualLinha->abaixo->linha < linha) {
            atualLinha = atualLinha->abaixo;
        }
        novaCelula->abaixo = atualLinha->abaixo;
        atualLinha->abaixo = novaCelula;

        Celula *atualColuna = matriz.cabeca;
        while (atualColuna->direita != matriz.cabeca && atualColuna->direita->coluna < coluna) {
            atualColuna = atualColuna->direita;
        }
        novaCelula->direita = atualColuna->direita;
        atualColuna->direita = novaCelula;
    }

    fclose(arquivo);
    return matriz;
}

Matriz somaMatrizes(Matriz A, Matriz B) {
    if (A.nlin != B.nlin || A.ncol != B.ncol) {
        printf("As matrizes têm dimensões diferentes. Não é possível somá-las.\n");
        exit(1);
    }

    Matriz C = criaMatriz(A.nlin, A.ncol);

    Celula *atualA = A.cabeca->abaixo;
    Celula *atualB = B.cabeca->abaixo;
    Celula *atualC = C.cabeca;

    while (atualA != A.cabeca && atualB != B.cabeca) {
        if (atualA->coluna == atualB->coluna) {
            atualC->valor = atualA->valor + atualB->valor;
            atualA = atualA->direita;
            atualB = atualB->direita;
        } else if (atualA->coluna < atualB->coluna) {
            atualC->valor = atualA->valor;
            atualA = atualA->direita;
        } else {
            atualC->valor = atualB->valor;
            atualB = atualB->direita;
        }

        if (atualA != A.cabeca || atualB != B.cabeca) {
            Celula *novaCelula = (Celula *)malloc(sizeof(Celula));
            novaCelula->linha = atualC->linha;
            novaCelula->coluna = atualC->coluna + 1;
            novaCelula->valor = 0.0;
            novaCelula->abaixo = novaCelula;
            novaCelula->direita = novaCelula;
            atualC->direita = novaCelula;
            atualC = atualC->direita;
        }
    }

    return C;
}


Matriz multiplicaMatrizes(Matriz A, Matriz B) {
    if (A.ncol != B.nlin) {
        printf("As matrizes não têm dimensões compatíveis para multiplicação.\n");
        exit(1);
    }

    Matriz C = criaMatriz(A.nlin, B.ncol);

    for (int i = 0; i < A.nlin; i++) {
        for (int j = 0; j < B.ncol; j++) {
            float soma = 0.0;

            Celula *atualA = A.cabeca->abaixo;
            Celula *atualB = B.cabeca->direita;

            while (atualA != A.cabeca && atualB != B.cabeca) {
                if (atualA->coluna == atualB->linha) {
                    soma += atualA->valor * atualB->valor;
                    atualA = atualA->direita;
                    atualB = atualB->abaixo;
                } else if (atualA->coluna < atualB->linha) {
                    atualA = atualA->direita;
                } else {
                    atualB = atualB->abaixo;
                }
            }

            if (soma != 0.0) {
                Celula *novaCelula = (Celula *)malloc(sizeof(Celula));
                novaCelula->linha = i;
                novaCelula->coluna = j;
                novaCelula->valor = soma;

                Celula *atualLinha = C.cabeca;
                while (atualLinha->abaixo != C.cabeca && atualLinha->abaixo->linha < i) {
                    atualLinha = atualLinha->abaixo;
                }
                novaCelula->abaixo = atualLinha->abaixo;
                atualLinha->abaixo = novaCelula;

                Celula *atualColuna = C.cabeca;
                while (atualColuna->direita != C.cabeca && atualColuna->direita->coluna < j) {
                    atualColuna = atualColuna->direita;
                }
                novaCelula->direita = atualColuna->direita;
                atualColuna->direita = novaCelula;
            }
        }
    }

    return C;
}


void liberaMatriz(Matriz matriz) {
    Celula *atualLinha = matriz.cabeca->abaixo;
    while (atualLinha != matriz.cabeca) {
        Celula *atualColuna = atualLinha->direita;
        while (atualColuna != atualLinha) {
            Celula *temp = atualColuna;
            atualColuna = atualColuna->direita;
            free(temp);
        }
        Celula *temp = atualLinha;
        atualLinha = atualLinha->abaixo;
        free(temp);
    }
    free(matriz.cabeca);
}

