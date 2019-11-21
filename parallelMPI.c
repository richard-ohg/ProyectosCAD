#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int** allocate_memory();
int* allocate_lineal_memory(int length);
void get_headers(FILE *f);
void set_headers(FILE *f, const char* file_signature, char* comment_buffer, int rows, int columns, int max_color);
void validate_args(int argc);
void check_file(FILE *f);
void free_mem (int **matriz);
int* gray_scale_matrix (int *rgb_matrix, int *green_matrix, int *blue_matrix);
int* negative_matrix (int *matriz);
void generate_grayscale_file (char* filename, int* matrix);
void generate_negative_file (char* filename, int* red, int* green, int* blue);

char comment_buffer[100];
char file_signature[3];
int MAX_COLOR_VALUE, ROWS, COLUMNS;
const char* GRAYSCALE_SIGNATURE = "P2";
const char* NEGATIVE_SIGNATURE = "P3";
static int PIXELS = 0;
int rank, world_size, set_counter;
double start, end;

int main(int argc, char *argv[]){
  
  validate_args(argc);

  FILE *original_file;
  int *rgb_matrix;
  int colorvalue;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  MPI_Barrier(MPI_COMM_WORLD); /* getting time */
  start = MPI_Wtime();

  original_file = fopen(argv[1], "r");
  check_file(original_file);
  get_headers(original_file);

  int count = 0;

  rgb_matrix = allocate_lineal_memory(PIXELS*3);

  while(fscanf(original_file,"%d\n",&colorvalue) != EOF){
    rgb_matrix[count] = colorvalue;
    ++count;
  }

  /***** GENERATING GRAYSCALE MATRIX *****/

  int num_pixels_per_proc = (PIXELS/world_size);

  int offset = num_pixels_per_proc;
  int remainder = PIXELS % world_size;

  int *local_rgb_matrix = allocate_lineal_memory(num_pixels_per_proc);
  
  MPI_Scatter(rgb_matrix, num_pixels_per_proc, MPI_INT, local_rgb_matrix,
              num_pixels_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
  
  int* grayscale_matrix_recv = allocate_lineal_memory(PIXELS);
  int* grayscale_matrix = allocate_lineal_memory(num_pixels_per_proc);
  int* displs = (int *)malloc(world_size*sizeof(int)); 
  int* recv_counts = (int *)malloc(world_size*sizeof(int)); 
  for (int i=0; i<world_size; ++i) { 
    displs[i] = i*num_pixels_per_proc+1; 
    recv_counts[i] = num_pixels_per_proc; 
  } 

  set_counter = 0;
  for(int i = (offset * rank)+1; i <= ((offset * rank) + offset); ++i) {
    grayscale_matrix[set_counter] = (rgb_matrix[i*3] + rgb_matrix[(i*3)+1] + rgb_matrix[(i*3)+2]) / 3;
    ++set_counter;
  }

  MPI_Gatherv(grayscale_matrix, num_pixels_per_proc, MPI_INT, 
    grayscale_matrix_recv, recv_counts, displs, MPI_INT, 0, MPI_COMM_WORLD);
  
  /****************** GENERATING GRAYSCALE FILE ********************/
  
  if (rank == 0) {
    FILE *grayscale_file;
    grayscale_file = fopen("lena_bn.pnm", "w");
    check_file(grayscale_file);
    set_headers(grayscale_file, GRAYSCALE_SIGNATURE, comment_buffer, ROWS, COLUMNS, MAX_COLOR_VALUE);
    for(int i=0; i < PIXELS; ++i) {
      fprintf(grayscale_file, "%d\n", grayscale_matrix_recv[i]);
    }
  }
  
  /************* GENERATE RGB-NEGATIVE MATRIX ******************/
  int num_lines_per_proc = (PIXELS*3)/world_size;
  int* rgb_negative_matrix_recv = allocate_lineal_memory(PIXELS*3);
  int* rgb_negative_matrix = allocate_lineal_memory(num_lines_per_proc);

  // Reassigning gather values
  for (int i=0; i<world_size; ++i) { 
    displs[i] = i*num_lines_per_proc+1; 
    recv_counts[i] = num_lines_per_proc; 
  } 

  set_counter = 0;
  for(int i = (num_lines_per_proc * rank)+1; i <= ((num_lines_per_proc * rank) + num_lines_per_proc); ++i) {
    rgb_negative_matrix[set_counter] = MAX_COLOR_VALUE - rgb_matrix[i];
    ++set_counter;
  }

  MPI_Gatherv(rgb_negative_matrix, num_lines_per_proc, MPI_INT, 
    rgb_negative_matrix_recv, recv_counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

  /************* GENERATING NEGATIVE FILE ******************/

  if (rank == 0) {
    FILE *negative_file;
    negative_file = fopen("lena_neg.pnm", "w");
    check_file(negative_file);
    set_headers(negative_file, NEGATIVE_SIGNATURE, comment_buffer, ROWS, COLUMNS, MAX_COLOR_VALUE);
    for(int i=0; i< PIXELS; ++i) {
      fprintf(negative_file, "%d\n%d\n%d\n", rgb_negative_matrix_recv[i*3], rgb_negative_matrix_recv[(i*3)+1], rgb_negative_matrix_recv[(i*3)+2]);
    }
  }

  if (rank == 0) {
    free(grayscale_matrix_recv);
    free(rgb_matrix);
  }
  free(grayscale_matrix);
  free(rgb_negative_matrix);

  MPI_Barrier(MPI_COMM_WORLD); /* ending */
  end = MPI_Wtime();

  MPI_Finalize();
  if (rank == 0) {
    printf("Runtime = %f\n", end-start);
  }
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

int* allocate_lineal_memory(int length) {
  return (int *)malloc(length * sizeof(int));
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