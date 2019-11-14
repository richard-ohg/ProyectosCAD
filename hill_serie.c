#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int** reserveMemoryMatrix(int rows, int columns);
void fillMatrix(int** matrix, int dimension);
void printMatrix(int** matrix,  int rows, int columns);
char* getAlphabet();
// Functions for read file and get a string clean for encrypt
char* readFile(char *filename);
void removeSpaces(char* string);
void replaceUnknownCharacters(char* string, char* alphabet);
// Function for fill string only if it's necessary
char* completeText(char* string, int numberMissingCharacters);
// Functions for convert string to vectors of numbers
int** separateStringToVectors(char* stringToSeparate, int numberOfVectors, int dimension, char* alphabet);
int* convertVectorToNumbers(char* vector, char* alphabet);
// 
int* multiplyVector(int** matrix, int *vector, int dimension, int module);


int main(int argc, char const *argv[]){
    int dimension, **nonSingularMatrix, *vector, lenghtAlphabet, **matrixOfVectors, module;
    char *stringToEncrypt, *alphabet, *textEncrypt;

    // Get Alphabet for encrypt
    alphabet = getAlphabet();
    lenghtAlphabet = strlen(alphabet);

    printf("Ingresa la dimension de la matriz: ");
    scanf("%d", &dimension);

    nonSingularMatrix = reserveMemoryMatrix(dimension, dimension);
    fillMatrix(nonSingularMatrix, dimension);
    // printMatrix(NonSingularMatrix, dimension, dimension);
    
    // Reading from file, return string
    stringToEncrypt = readFile("test.txt");
    // puts(stringToEncrypt); // test

    // Removing blank spaces for string to encrypt, pass by value is used
    removeSpaces(stringToEncrypt);
    // puts(stringToEncrypt); // test

    // check if string is complete, otherwise it is filled
    module = strlen(stringToEncrypt) % dimension;
    if(module != 0){
        printf("Necesita completarse\n");
        int numberMissingCharacters = dimension - module;
        stringToEncrypt = completeText(stringToEncrypt, numberMissingCharacters);
        // puts(stringToEncrypt); // test
        // printf("length after complete text: %d\n", strlen(stringToEncrypt)); // test
    }
    // puts(stringToEncrypt); // test

    // up to this point, ready string separate in vectors of numbers for multiply with non singular matrix
    int numberOfVectors = strlen(stringToEncrypt)/dimension;
    matrixOfVectors = separateStringToVectors(stringToEncrypt, numberOfVectors, dimension, alphabet);

    printf("Before multiply\n"); // test
    printMatrix(matrixOfVectors, numberOfVectors, dimension); // test

    for (int i = 0; i < numberOfVectors; i++){
        matrixOfVectors[i] = multiplyVector(nonSingularMatrix, matrixOfVectors[i], dimension, strlen(alphabet));
    }
    printf("After multiply\n"); // test
    printMatrix(matrixOfVectors, numberOfVectors, dimension); // test


    return 0;
}

int** reserveMemoryMatrix(int rows, int columns){
    int **matrix = (int **) calloc(rows, sizeof(int*));

    for (int i = 0; i < rows; i++)
        matrix[i] = (int*) calloc(columns, sizeof(int));
    
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
    // return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-!?_";
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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
/* Missing implements
void replaceUnknownCharacters(char* string, char* alphabet){
    for (int i = 0; i < strlen(string); i++){
        if (!checkIfExist(string[i], alphabet)){
            string[i]
        }
             
    }  
}

bool checkIfExist(char character, char* alphabet){
    for (int j = 0; j < strlen(alphabet); j++){
        if (character == alphabet[j]){
            return true;
        }   
    }
    return false;
} */

char* completeText(char* string, int numberMissingCharacters){
    char *new_string = (char*) calloc (strlen(string)+numberMissingCharacters, sizeof(char));

    // char complements[numberMissingCharacters]; // It doesn't work
    char *complements = (char*) calloc (numberMissingCharacters, sizeof(char));

    for (int i = 0; i < numberMissingCharacters; i++)
        complements[i] = 'X';
    // puts(complements); // test

    strcpy(new_string, string);
    strcat(new_string, complements);

    return new_string;
}

int** separateStringToVectors(char* stringToSeparate, int numberOfVectors, int dimension, char* alphabet){
    char **arrayOfStrings;
    int **matrixOfNumbers;
    
    // Reserve memory for array containing string vectors
    arrayOfStrings = (char**) calloc(numberOfVectors, sizeof(char*));
    
    for (int i = 0; i < numberOfVectors; i++)
        arrayOfStrings[i] = (char*) calloc(dimension, sizeof(char));
    
    // Resever memory for matrix containing numbers vectors
    matrixOfNumbers = reserveMemoryMatrix(numberOfVectors, dimension);

    // Separate string each 'dimension' elements
    for (int i = 0; i < numberOfVectors; i++){
        for (int j = 0; j < dimension; j++){
            arrayOfStrings[i][j] = stringToSeparate[(i*dimension)+j]; 
        }     
    }

    // Convert each string to numbers according to the alphabet
    for (int i = 0; i < numberOfVectors; i++)
        matrixOfNumbers[i] = convertVectorToNumbers(arrayOfStrings[i], alphabet);

    // printf("Test of matrix\n"); // test
    // printMatrix(matrixOfNumbers, numberOfVectors, dimension); // test
    
    return matrixOfNumbers;

}

int* convertVectorToNumbers(char* vector, char* alphabet){
    int* vectorOFNumbers = (int*) calloc(strlen(vector), sizeof(int));

    for (int i = 0; i < strlen(vector); i++){
        for (int j = 0; j < strlen(alphabet); j++){
            if (vector[i] == alphabet[j]){
                // printf("El indice del caracter %c dentro del alfabeto es: %d\n", vector[i], j); // test
                vectorOFNumbers[i] = j;
            }  
        }  
    }
    // printf("\n"); // test 

    return vectorOFNumbers;  
}

int* multiplyVector(int** matrix, int *vector, int dimension, int module){
    //Reserving memory for new vector
    int *resultVector = (int*) calloc(dimension, sizeof(int));
    int i, j, tmp;

    //Iterating the matrix and multiplying by the vector pointer
    for(i=0; i < dimension; i++) {
        for(j=0; j < dimension; j++) {
            *(resultVector + i) += (matrix[i][j] * (*(vector + j)));
        }
        // Apply module 
        *(resultVector+i) = *(resultVector+i) % module; 
    }
    
    return resultVector;
}