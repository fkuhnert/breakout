#include <stdio.h>
#include <string.h>
int main (int argc, char **argv[]){
  char texto[80], arquivo[80];
  FILE *file;
  int i, space;
  float nota2;
  printf("Escreva o nome do arquivo\n");
  fgets(arquivo, 80, stdin);
  arquivo[strlen(arquivo) - 1] = '\0';
  if ((file = fopen(arquivo, "w")) == NULL){
    printf("Erro na leitura de arquivo\n");
  }
  for(i = 0; i < 4; i++){
    printf("Escreva o nome do aluno %i\n", i+1);
    fgets(texto, 80, stdin);
    texto[strlen(texto) - 1] = '\0';
    printf("Escreva as notas do aluno %i\n", i+1);
    scanf("%f ", &nota2);
    fprintf(file, "%s ", texto);
    for(space = 0; space < 20-strlen(texto); space++){
      fprintf(file, " ");
    }
    fprintf(file, "%4.1f\n", nota2);
  }
  fclose(file);
  return 0;
}
