#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** reserveMemoryMatrix(int dimension);
void fillMatrix(int** matrix, int dimension);
void printMatrix(int** matrix, int dimension);
char* getAlphabet();
char* readFile(char *filename);
char** separateStringToVectors(char* string, int dimension);
void removeSpaces(char* string);

int main(int argc, char const *argv[]){
    int dimension, **matrix, *vector, lenghtAlphabet;
    char *stringToEncrypt, *alphabet;

    // Get Alphabet for encrypt
    alphabet = getAlphabet();
    lenghtAlphabet = strlen(alphabet);

    printf("Ingresa la dimension de la matriz: ");
    scanf("%d", &dimension);

    matrix = reserveMemoryMatrix(dimension);
    fillMatrix(matrix, dimension);
    // printMatrix(matrix, dimension);
    
    // Reading from file, return string
    stringToEncrypt = readFile("test.txt");
    // puts(stringToEncrypt); // test

    // Removing blank spaces for encrypt
    removeSpaces(stringToEncrypt);
    // puts(stringToEncrypt); // test

    separateStringToVectors(stringToEncrypt, dimension);


    // for (int i = 0; i < lenghtAlphabet; i++){
    //     if (alphabet[i] == ' '){
    //         printf("Es espacio en el indice: %d", i);
    //     }
    // }



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
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-!?_";
}

char* readFile(char *fileName){
    char *buffer = NULL;
    int string_size, read_size;
    FILE *file = fopen(fileName, "r");

    if (file){
        // Seek the last byte of the file
        fseek(file, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(file);
        // go back to the start of the file
        rewind(file);

        // Allocate a string that can hold it all
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

        // Read it all in one operation
        read_size = fread(buffer, sizeof(char), string_size, file);

        // fread doesn't set it so put a \0 in the last position
        // and buffer is now officially a string
        buffer[string_size] = '\0';

        if(string_size != read_size){
            // Something went wrong, throw away the memory and set
            // the buffer to NULL
            free(buffer);
            buffer = NULL;
        }

        // Always remember to close the file.
        fclose(file);
    }

    return buffer;
}

void removeSpaces(char* string) {
    const char* d = string;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*string++ = *d++);
}

char** separateStringToVectors(char* string, int dimension){
    int module = strlen(string) % dimension;
    int numberOfVectors = strlen(string)/dimension;
    char** matrixOfVectors;

    // matrixOfVectors = (char**) calloc(numberOfVectors,sizeof(char*));
    // for
    printf("Module: %d\n",module);
    printf("Length of string: %d\n",strlen(string));
    if(offset != 0){
        printf("necesita acompletarse");
    }else{
        printf("Esta completo");
    }
}
