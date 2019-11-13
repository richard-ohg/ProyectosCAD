#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** reserveMemoryMatrix(int dimension);
void fillMatrix(int** matrix, int dimension);
void printMatrix(int** matrix,  int rows, int columns);
char* getAlphabet();
char* readFile(char *filename);
char** separateStringToVectors(char* string, int dimension);
void removeSpaces(char* string);
char* completeText(char* string, int numberMissingCharacters);


int main(int argc, char const *argv[]){
    int dimension, **NonSingularMatrix, *vector, lenghtAlphabet;
    char *stringToEncrypt, *alphabet, **matrixOfVectors;

    // Get Alphabet for encrypt
    alphabet = getAlphabet();
    lenghtAlphabet = strlen(alphabet);

    printf("Ingresa la dimension de la matriz: ");
    scanf("%d", &dimension);

    NonSingularMatrix = reserveMemoryMatrix(dimension);
    // fillMatrix(NonSingularMatrix, dimension);
    // printMatrix(NonSingularMatrix, dimension, dimension);
    
    // Reading from file, return string
    stringToEncrypt = readFile("test.txt");
    // puts(stringToEncrypt); // test

    // Removing blank spaces for encrypt
    removeSpaces(stringToEncrypt);
    // puts(stringToEncrypt); // test

    // up to this point, ready string separate in vectors
    matrixOfVectors = separateStringToVectors(stringToEncrypt, dimension);

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

void printMatrix(int** matrix, int rows, int columns){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
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
    char* stringToSeparate;
    char** matrixOfVectors;

    if(module != 0){
        printf("necesita completarse\n");
        int numberMissingCharacters = dimension - module;
        stringToSeparate = completeText(string, numberMissingCharacters);
        // puts(stringToSeparate); // test
        // printf("length after complete text: %d\n", strlen(stringToSeparate)); // test
    }else{
        stringToSeparate = string;
    }
    int numberOfVectors = strlen(stringToSeparate)/dimension;
    // puts(stringToSeparate); // test
    
    matrixOfVectors = (char**) calloc(numberOfVectors, sizeof(char*));

    for (int i = 0; i < numberOfVectors; i++){
        matrixOfVectors[i] = (char*) calloc(dimension, sizeof(char));
    }
    

    for (int i = 0; i < numberOfVectors; i++){
        for (int j = 0; j < dimension; j++){
            matrixOfVectors[i][j] = stringToSeparate[(i*dimension)+j]; 
        }     
    }

// Test - Printing matrix
    // for (int i = 0; i < numberOfVectors; i++){
    //     for (int j = 0; j < dimension; j++){
    //         printf("%c ", matrixOfVectors[i][j]); 
    //     }
    //     printf("\n");
    // }

    return matrixOfVectors;

}

char* completeText(char* string, int numberMissingCharacters){
    char *new_string = (char*) calloc (strlen(string)+numberMissingCharacters, sizeof(char));

    // char complements[numberMissingCharacters]; // It doesn't work
    char *complements = (char*) calloc (numberMissingCharacters, sizeof(char));

    for (int i = 0; i < numberMissingCharacters; i++){
        complements[i] = 'X';
    }
    // puts(complements); // test

    strcpy(new_string, string);
    strcat(new_string, complements);

    return new_string;
}