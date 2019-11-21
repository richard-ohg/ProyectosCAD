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
void replaceAndremoveSpaces(char* string,char* alphabet);
int checkIfExist(char character, char* alphabet);
// Function for fill string only if it's necessary
char* completeText(char* string, int numberMissingCharacters);
// Functions for convert string to vectors of numbers
int** separateStringToVectors(char* stringToSeparate, int numberOfVectors, int dimension, char* alphabet);
int* convertVectorToNumbers(char* vector, char* alphabet);

int* multiplyVector(int** matrix, int *vector, int dimension, int module);

char* convertNumbersToStrign(int *vector, char* alphabet, int dimension);
char* encryptVector(int** matrix, char * alphabet, int dimension, int numberOfVectors);
void printFile(char* fileName, char* string);

int main(int argc, char *argv[]){
    int dimension, **nonSingularMatrix, *vector, lenghtAlphabet, **matrixOfVectors, module;
    char *stringToEncrypt, *alphabet, *textEncrypt;

    if(argc != 2){
        printf("numero de parametros incorrecto:  program <file>\n");
        return 0;
    }

    // Get Alphabet for encrypt
    alphabet = getAlphabet();
    lenghtAlphabet = strlen(alphabet);

    printf("Ingresa la dimension de la matriz: ");
    scanf("%d", &dimension);

    nonSingularMatrix = reserveMemoryMatrix(dimension, dimension);
    fillMatrix(nonSingularMatrix, dimension);
    // printMatrix(NonSingularMatrix, dimension, dimension);

    // Reading from file, return string
    stringToEncrypt = readFile(argv[1]);
    // puts(stringToEncrypt); // test

    // Removing blank spaces for string to encrypt, pass by value is used
    replaceAndremoveSpaces(stringToEncrypt,alphabet);
    //puts(stringToEncrypt); // test

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

    //printf("Before multiply\n"); // test
    //printMatrix(matrixOfVectors, numberOfVectors, dimension); // test

    for (int i = 0; i < numberOfVectors; i++){
        matrixOfVectors[i] = multiplyVector(nonSingularMatrix, matrixOfVectors[i], dimension, strlen(alphabet));
    }
    //printf("After multiply\n"); // test
    //printMatrix(matrixOfVectors, numberOfVectors, dimension); // test

    textEncrypt = encryptVector(matrixOfVectors, alphabet, dimension, numberOfVectors);

    //puts(textEncrypt);
    printFile("salida.txt",textEncrypt);



    return 0;
}


char* getAlphabet(){
    // return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-!?_";
    // return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
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

void printFile(char* fileName, char* string){
    FILE * file = fopen(fileName, "w");
    if(file){
        fputs(string,file);
        fclose(file);
    }
}

void replaceAndremoveSpaces(char* str, char* alphabet) {
    size_t str_len = strlen(str);
    char result[str_len];
    size_t p = 0;
    size_t i = 0;
    for (i = 0; i < str_len; ++i) {
        if (str[i] != ' ') {
            if(str[i] == 10 || str[i] == 11 || str[i] == 13 || checkIfExist(str[i], alphabet)){
                result[p] = str[i];
            }else{
                switch((unsigned int)str[i]){
                    case 165: //Ñ
                      result[p] = 'N';
                      break;
                    case 164: //ñ
                      result[p] = 'n';
                      break;
                    default:
                      result[p] = 'X';
                }
            }
            p++;
        }
    }
    //puts(result);  //test
    if (p < str_len){
        str[p] = '\0';
    }
    for (i = 0; i < p; ++i) {
        str[i] = result[i];
    }
}

int checkIfExist(char character, char* alphabet){
    for (int j = 0; j < strlen(alphabet); j++){
        if (character == alphabet[j]){
            return 1;
        }
    }
    return 0;
}

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

char* encryptVector(int** matrix, char * alphabet, int dimension, int numberOfVectors){
    char* ev = (char*) calloc(dimension*numberOfVectors,sizeof(char));
    for(int i=0; i<numberOfVectors; i++){
        ev = strcat(ev, convertNumbersToStrign(matrix[i],alphabet,dimension));
    }
    return ev;
}

char* convertNumbersToStrign(int *vector, char* alphabet, int dimension){
    char* str = (char*) calloc(dimension,sizeof(char));
    int pos;
    for(int i = 0 ; i<dimension ; i++){
        //printf("Numero:  %d, Caracter %c\n",vector[i],alphabet[vector[i]] );
        str[i] = alphabet[vector[i]];
    }
    //puts(str);  //test
    return str;
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
