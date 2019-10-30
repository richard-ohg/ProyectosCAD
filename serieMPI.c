#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){

    FILE *file,*copy;
    char buffer[100];

    file = fopen("lena_color.pnm", "r");
    copy = fopen("lena_copy.pnm","w");

    if(file == NULL){
        printf("El archivo no existe");
        exit(1);
    }

// Read line for line
    while (fgets(buffer, 100, file) != NULL){
        fputs(buffer,copy);
    }
    
    return 0;
}
