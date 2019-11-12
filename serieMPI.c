#include <stdio.h>
#include <stdlib.h>

int** allocate_memory();
int* allocate_lineal_memory(int length);
void get_headers(FILE *f);
void set_headers(FILE *f, const char* file_signature, char* comment_buffer, int rows, int columns, int max_color);
void validate_args(int argc);
void check_file(FILE *f);
void free_mem (int **matriz);
int* gray_scale_matrix (int *red_matrix, int *green_matrix, int *blue_matrix);
int* negative_matrix (int *matriz);
void generate_grayscale_file (char* filename, int* matrix);
void generate_negative_file (char* filename, int* red, int* green, int* blue);

char comment_buffer[100];
char file_signature[3];
int MAX_COLOR_VALUE, ROWS, COLUMNS;
const char* GRAYSCALE_SIGNATURE = "P2";
const char* NEGATIVE_SIGNATURE = "P3";
static int PIXELS = 0;

int main(int argc, char *argv[]){
  validate_args(argc);
  FILE *file;
  int r,g,b, count = 0;
  int *red_matrix, *green_matrix, *blue_matrix;
  file = fopen(argv[1], "r");
  check_file(file);
  get_headers(file);

  red_matrix = allocate_lineal_memory(PIXELS);
  green_matrix = allocate_lineal_memory(PIXELS);
  blue_matrix = allocate_lineal_memory(PIXELS);

  //printf("header: %s\nCommentario: %sTamaño: %d X %d\nMaximo: %d\n",header,comment_buffer,COLUMNS,ROWS,MAX_COLOR_VALUE);

  while(fscanf(file,"%d\n%d\n%d\n",&r,&g,&b) != EOF){
    red_matrix[count] = r;
    green_matrix[count] = g;
    blue_matrix[count] = b;
    ++count;
  }

  generate_grayscale_file("lena_bn.pnm", gray_scale_matrix(red_matrix,green_matrix,blue_matrix));
  generate_negative_file("lena_neg.pnm", negative_matrix(red_matrix), negative_matrix(green_matrix), negative_matrix(blue_matrix));

  free(red_matrix);
  free(green_matrix);
  free(blue_matrix);
  return 0;
}

/** 
 * Function that gets the file .pnm headers
 * Following the structure: 
 * [File_signature]
 * [Here goes a comment]
 * [width] [height]
 * [max RGB value]
 */
void get_headers(FILE *file) {
  fscanf(file,"%s\n", file_signature);
  fgets(comment_buffer, 100, file);
  fscanf(file,"%d %d\n", &COLUMNS, &ROWS);
  PIXELS = ROWS * COLUMNS;
  fscanf(file,"%d\n",&MAX_COLOR_VALUE);
}

void generate_negative_file(char* filename, int* red, int* green, int* blue){
  FILE *file;
  file = fopen(filename, "w");
  check_file(file);
  set_headers(file, NEGATIVE_SIGNATURE, comment_buffer, ROWS, COLUMNS, MAX_COLOR_VALUE);
  for(int i=0; i< PIXELS; ++i) {
    fprintf(file, "%d\n%d\n%d\n", red[i], green[i], blue[i]);
  }
}

void generate_grayscale_file(char* filename, int* matrix){
  FILE *file;
  file = fopen(filename,"w");
  check_file(file);
  set_headers(file, GRAYSCALE_SIGNATURE, comment_buffer, ROWS, COLUMNS, MAX_COLOR_VALUE);
  for(int i=0; i < PIXELS; ++i) {
    fprintf(file, "%d\n",matrix[i]);
  }
}

int** allocate_memory() {
  int **x;
  int i;
  x = (int **)malloc(COLUMNS*sizeof(int*));
  for (i=0;i<COLUMNS;++i) {
    x[i] = (int*)malloc(ROWS*sizeof(int));
  }
  return x;
}

int* allocate_lineal_memory(int length) {
  return (int *)malloc(length * sizeof(int));
}

int* gray_scale_matrix(int *red_matrix, int *green_matrix, int *blue_matrix){
  int* matrix = allocate_lineal_memory(PIXELS);
  for(int i = 0; i < PIXELS; ++i) {
    matrix[i] = (red_matrix[i] + green_matrix[i] + blue_matrix[i]) / 3;
  }
  return matrix;
}

int* negative_matrix(int* matrix) {
  int* negative_matrix = allocate_lineal_memory(PIXELS);
  for(int i = 0; i < PIXELS; ++i) {
    negative_matrix[i] = MAX_COLOR_VALUE - matrix[i];
  }
  return negative_matrix;
}

void validate_args(int argc) {
  if(argc < 2){
    printf("Modo de uso:  ejecutable <imagen.pnm>\n");
    exit(1);
  }
}

void check_file(FILE *file) {
  if(file == NULL){
    printf("Error al procesar");
    exit(1);
  }
}

void set_headers(FILE *file, const char* file_signature, char* comment_buffer, int rows, int columns, int max_color_value) {
  fputs(file_signature,file);
  fputs("\n",file);
  fputs(comment_buffer,file);
  fprintf(file,"%d %d\n", columns, rows);
  fprintf(file, "%d\n", max_color_value);
}