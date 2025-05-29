#include "parser.h"

/* mude diretorio para onde vai ficar o arquivo de senhas */
#define PASS_FILE "./senhas.conf"


int main(int argc, char *argv[])
{

  lerarquivo(PASS_FILE);

  return EXIT_SUCCESS;
}
