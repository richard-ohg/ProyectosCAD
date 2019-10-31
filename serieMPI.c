#include <stdio.h>
#include <stdlib.h>

int** asigna();
void libera(int **matriz);
int** blanco_negro(int **mR, int **mG, int **mB);
int** invertir_color(int **matriz);
void escribeBN(int** matriz);
void escribeCI(int** mr, int** mg, int** mb);

char buffer[100];
char heder[3];
int columnas, filas,maximo;

int main(int argc, char const *argv[]){

    FILE *file,*copy;

    int r,g,b,i,j;
    int **mR, **mG,**mB, **bn, **mIR, **mIG,** mIB;
    file = fopen("lena_color.pnm", "r");
    copy = fopen("lena_copy.pnm","w");



    if(file == NULL){
        printf("El archivo no existe");
        exit(1);
    }
    fscanf(file,"%s\n",heder);
    fgets(buffer, 100, file);
    fscanf(file,"%d %d\n",&columnas, &filas);
    fscanf(file,"%d\n",&maximo);

    mR = asigna();
    mG = asigna();
    mB = asigna();

    printf("Heder: %s\nCommentario: %sTamaño: %d X %d\nMaximo: %d\n",heder,buffer,columnas,filas,maximo);

    int auxC = 0, auxF =0;
    while(fscanf(file,"%d\n%d\n%d\n",&r,&g,&b) != EOF){
    //for(i =0 ; i< columnas*filas;i++){
      //fscanf(file,"%d\n%d\n%d\n",&r,&g,&b);
      mR[auxC][auxF] = r;
      mG[auxC][auxF] = g;
      mB[auxC][auxF] = b;

      //printf("%d   %d   %d    %d   %d   %d\n",r,g,b,mR[auxC][auxF],mG[auxC][auxF],mB[auxC][auxF]);
      //printf("%d   %d\n",auxC, auxF);
      //auxC++;
      auxF++;

      if(auxF == filas){
        auxF = 0;
        auxC++;
      }
    }

    bn = blanco_negro(mR,mG,mB);
    mIR = invertir_color(mR);
    mIG = invertir_color(mG);
    mIB = invertir_color(mB);

    escribeBN(bn);
    escribeCI(mIR,mIG,mIB);

    libera(mR);
    libera(mG);
    libera(mB);
    libera(bn);
    libera(mIR);
    libera(mIG);
    libera(mIB);

    return 0;
}

void escribeCI(int** mr, int** mg, int** mb){
  FILE *file;
  file = fopen("lena_inv.pnm","w");
  if(file == NULL){
      printf("El archivo no existe");
      return;
  }

  fputs("P3\n",file);
  fputs(buffer,file);
  fprintf(file,"%d %d\n",columnas,filas);
  fprintf(file, "%d\n",maximo);
  int i, j;
  for(i=0; i<columnas;i++)
    for(j=0; j<filas; j++)
      fprintf(file, "%d\n%d\n%d\n",mr[i][j],mg[i][j],mb[i][j]);
}

void escribeBN(int** matriz){
  FILE *file;
  file = fopen("lena_bn.pnm","w");
  if(file == NULL){
      printf("El archivo no existe");
      return;
  }

  fputs("P2\n",file);
  fputs(buffer,file);
  fprintf(file,"%d %d\n",columnas,filas);
  fprintf(file, "%d\n",maximo);
  int i, j;
  for(i=0; i<columnas;i++)
    for(j=0; j<filas; j++)
      fprintf(file, "%d\n",matriz[i][j]);
}

int** asigna(){
  int **x;
  int i;
  x = (int **)malloc(columnas*sizeof(int*));

	for (i=0;i<columnas;i++)
		x[i] = (int*)malloc(filas*sizeof(int));

  return x;
}

int** blanco_negro(int **mR, int **mG, int **mB){
  int **bn = asigna();
  int i,j;

  for(i = 0; i< columnas; i++)
    for(j = 0; j < filas; j++){
      //printf("%d  %d   %d\n",mR[i][j],mG[i][j],mB[i][j]);
      bn[i][j] = (mR[i][j] + mG[i][j] + mB[i][j]) / 3;
    }
      //printf("%d\n",(mR[i][j] + mG[i][j] + mB[i][j]) / 3 );


  return bn;
}

int** invertir_color(int **matriz){
  int **alt = asigna();
  int i,j;

  for(i = 0; i< columnas; i++)
    for(j = 0; j < filas; j++)
      alt[i][j] = maximo - matriz[i][j];

  return alt;
}

void libera(int **matriz){
  int i;
  for(i=0; i<columnas;i++)
    free(matriz[i]);

  free(matriz);
}
