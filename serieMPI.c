#include <stdio.h>
#include <stdlib.h>

int** asigna(int columnas, int filas);
void libera(int **matriz, int columnas);
int** blanco_negro(int **mR, int **mG, int **mB, int columnas, int filas );
int** invertir_color(int **matriz, int columnas, int filas, int maximo);

int main(int argc, char const *argv[]){

    FILE *file,*copy;
    char buffer[100]; //comentario
    char heder[3];
    int columnas, filas,maximo;
    int r,g,b,i;
    int **mR, **mG,**mB;
    file = fopen("lena_color.pnm", "r");
    copy = fopen("lena_copy.pnm","w");



    if(file == NULL){
        printf("El archivo no existe");
        exit(1);
    }

    // Read line for line
    fscanf(file,"%s\n",heder);
    fgets(buffer, 100, file);
    fscanf(file,"%d %d\n",&columnas, &filas);
    fscanf(file,"%d\n",&maximo);

    mR = asigna(columnas,filas);
    mG = asigna(columnas,filas);
    mB = asigna(columnas,filas);

    printf("Heder: %s\nCommentario: %sTamaño: %d X %d\nMaximo: %d\n",heder,buffer,columnas,filas,maximo);

    int auxC = 0, auxF =0;
    while(fscanf(file,"%d\n%d\n%d\n",&r,&g,&b) != EOF){
      mR[auxC][auxF] = r;
      mG[auxC][auxF] = g;
      mB[auxC][auxF] = b;

      auxC++;
      auxF++;

      if(auxC == columnas)
        auxC = 0;

      if(auxF == filas)
      auxF = 0;
    }


    libera(mR,columnas);
    libera(mG,columnas);
    libera(mB,columnas);

    return 0;
}

int** asigna(int columnas, int filas){
  int **x;
  int i;
  x = (int **)malloc(columnas*sizeof(int*));

	for (i=0;i<columnas;i++)
		x[i] = (int*)malloc(filas*sizeof(int));

  return x;
}

int** blanco_negro(int **mR, int **mG, int **mB, int columnas, int filas ){
  int **bn = asigna(columnas,filas);
  int i,j;

  for(i = 0; i< columnas; i++)
    for(j = 0; j < filas; j++)
      bn[i][j] = (mR[i][j] + mG[i][j] + mB[i][j]) / 3;

  return bn;
}

int** invertir_color(int **matriz, int columnas, int filas, int maximo){
  int **alt = asigna(columnas,filas);
  int i,j;

  for(i = 0; i< columnas; i++)
    for(j = 0; j < filas; j++)
      alt[i][j] = maximo - matriz[i][j];

  return alt;
}

void libera(int **matriz, int columnas){
  int i;
  for(i=0; i<columnas;i++)
    free(matriz[i]);

  free(matriz);
}
