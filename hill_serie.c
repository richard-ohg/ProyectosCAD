#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** reserveMemory(int dimension);
void fillMatrix(int** matrix, int dimension);
void printMatrix(int** matrix, int dimension);
char* getAlphabet();

int main(int argc, char const *argv[]){
    int dimension, **matrix, *vector;
    char *stringToEncrypt, *alphabet;

    alphabet = getAlphabet();

    // printf("Ingresa la dimension de la matriz: ");
    // scanf("%d", &dimension);

    // matrix = reserveMemory(dimension);
    // fillMatrix(matrix, dimension);
    // // printMatrix(matrix, dimension);

    // for (int i = 0; i < strlen(alphabet)-1; i++){
    //     // if (strcmp(alphabet[i], " ") == 0){
    //     //     printf("Es espacio");
    //     // }
    //     if (alphabet[i] == ' ')
    //     {
    //         printf("Es espacio es el indice: %d",i);
    //     }
        
        
    //     // printf("%c ",alphabet[i]);
    // }
    printf("alphabet[56]: %c", alphabet[57]);
    printf("length %d", strlen(alphabet));
    



    return 0;
}

int** reserveMemoryMatrix(int dimension){
    int **matrix = (int **) calloc(dimension, sizeof(int*));

    for (int i = 0; i < dimension; i++)
        matrix[i] = (int*) calloc(dimension, sizeof(int));
    
    return matrix;
}

void fillMatrix(int** matrix, int dimension){
    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            printf("Ingresa el elemento [%d][%d] de la matriz: ",i,j);
            scanf("%d",&matrix[i][j]);    
        }
    }
}

void printMatrix(int** matrix, int dimension){
    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            printf("%d ",matrix[i][j]);   
        }
        printf("\n");
    }
}

char* getAlphabet(){
    return "ABCDEFGHIJKLMNÑOPQRSTUVWXYZabcdefghijklmnñopqrstuvwxyz .,;-¡!¿?_";
}

