#include <stdio.h>
#include <stdlib.h>

int** allocate_memory();
void free_mem (int **matriz);
int** gray_scale_matrix (int **red_matrix, int **green_matrix, int **blue_matrix);
int** negative (int **matriz);
void generate_grayscale_file (char* filename, int** matriz);
void generate_negative_file (char* filename, int** red, int** green, int** blue);

char buffer[100];
char header[3];
int columns, rows, max_color_value;
const char* GRAY_SCALE_HEADER = "P2";
const char* NEGATIVE_HEADER = "P3";

int main(int argc, char const *argv[]){
    if(argc < 2){
      printf("Modo de uso:  ejecutable <imagen.pnm>\n");
      return 1;
    }
    FILE *file;
    int r,g,b,i,j;
    int **red_matrix, **green_matrix, **blue_matrix;
    file = fopen(argv[1], "r");
    if(file == NULL){
        printf("El archivo no existe");
        exit(1);
    }
    fscanf(file,"%s\n",header);
    fgets(buffer, 100, file);
    // Getting file resolution and max len
    fscanf(file,"%d %d\n",&columns, &rows);
    fscanf(file,"%d\n",&max_color_value);

    red_matrix = allocate_memory();
    green_matrix = allocate_memory();
    blue_matrix = allocate_memory();

    //printf("header: %s\nCommentario: %sTamaño: %d X %d\nMaximo: %d\n",header,buffer,columns,rows,max_color_value);

    int auxC = 0, auxF =0;
    while(fscanf(file,"%d\n%d\n%d\n",&r,&g,&b) != EOF){
      red_matrix[auxC][auxF] = r;
      green_matrix[auxC][auxF] = g;
      blue_matrix[auxC][auxF] = b;

      auxF++;
      if(auxF == rows){
        auxF = 0;
        auxC++;
      }
    }

    generate_grayscale_file("lena_bn.pnm", gray_scale_matrix(red_matrix,green_matrix,blue_matrix));
    generate_negative_file("lena_neg.pnm", negative(red_matrix), negative(green_matrix), negative(blue_matrix));

    free_mem(red_matrix);
    free_mem(green_matrix);
    free_mem(blue_matrix);

    return 0;
}

void generate_negative_file(char* nombre,int** mr, int** mg, int** mb){
  FILE *file;
  file = fopen(nombre,"w");
  if(file == NULL){
      printf("El archivo no existe");
      return;
  }
  fputs(NEGATIVE_HEADER,file);
  fputs("\n",file);
  fputs(buffer,file);
  fprintf(file,"%d %d\n",columns,rows);
  fprintf(file, "%d\n",max_color_value);
  int i, j;
  for(i=0; i<columns;i++)
    for(j=0; j<rows; j++)
      fprintf(file, "%d\n%d\n%d\n",mr[i][j],mg[i][j],mb[i][j]);
}

void generate_grayscale_file(char* nombre,int** matriz){
  FILE *file;
  file = fopen(nombre,"w");
  if(file == NULL){
    printf("El archivo no existe");
    return;
  }
  fputs(GRAY_SCALE_HEADER,file);
  fputs("\n",file);
  fputs(buffer,file);
  fprintf(file,"%d %d\n",columns,rows);
  fprintf(file, "%d\n",max_color_value);
  int i, j;
  for(i=0; i<columns;i++) {
    for(j=0; j<rows; j++) {
      fprintf(file, "%d\n",matriz[i][j]);
    }
  }
}

int** allocate_memory(){
  int **x;
  int i;
  x = (int **)malloc(columns*sizeof(int*));
	for (i=0;i<columns;i++) {
		x[i] = (int*)malloc(rows*sizeof(int));
  }
  return x;
}

int** gray_scale_matrix(int **red_matrix, int **green_matrix, int **blue_matrix){
  int **bn = allocate_memory();
  int i,j;
  for(i = 0; i< columns; i++) {
    for(j = 0; j < rows; j++){
      bn[i][j] = (red_matrix[i][j] + green_matrix[i][j] + blue_matrix[i][j]) / 3;
    }
  }
  return bn;
}

int** negative(int **matriz) {
  int **negative_matrix = allocate_memory();
  int i,j;
  for(i = 0; i< columns; i++)
    for(j = 0; j < rows; j++)
      negative_matrix[i][j] = max_color_value - matriz[i][j];
  return negative_matrix;
}

void free_mem(int **matriz) {
  int i;
  for(i=0; i<columns;i++) {
    free(matriz[i]);
  }
  free(matriz);
}