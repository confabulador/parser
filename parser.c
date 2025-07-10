#include "parser.h"

/* mude diretorio para onde vai ficar o arquivo de senhas */
#define PASS_FILE "./senhas.conf"

/* mude esses valores para o que desejar */
#define INIT "f%$#"
#define SEPARATOR "separador"
#define END "fim"

int main(int argc, char *argv[])
{
  
  // isso vai mudar
  char *usuario = "teste";

  AuthData *info = lerarquivo(PASS_FILE, INIT, SEPARATOR, END, usuario);
  if(info == INVALID_DATA){
    printf("não foi encontrnado em uma entrada valida no arquivo padrão\n");
    return 1;
  }
  
  // isso não vai exisitir no final
  printf("valores retornados: %s - %s - %s\n",info -> user, info -> pwhash, info -> script);
  
  /* liberando a memoria alocado da struct AuthData */
  limparstruct(info);
  
  return EXIT_SUCCESS;
}
