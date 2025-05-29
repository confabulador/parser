#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG


struct SecondAuthData{
    char *user;         /* o nome do usuario na linha */
    char **pwhashs;     /* os hash já calculado da(s) senha(s) de segurança para o usuario */
    char **scripts;     /* os scrips/programas dos repectivos hash de segurança */
    size_t len;         /* a quantidade de hashs e scripts associados ao usuario que esta tentando logar */
};




char *parsing(char *linebuff,char *init,char *separator,char *end){
    size_t lenline = strlen(linebuff);          /* tamanho linha */
    size_t leninit = strlen(init);              /* tamanho da formatação inicial */
    size_t lenend = strlen(end);                /* tamanho da formatação final */
    size_t lenseparator = strlen(separator);    /* tamanho da formatação de separação */
    
    struct SecondAuthData info;

    char *ponteiro = NULL;
    
    ponteiro = strstr(linebuff,init); /* string iniciando a partir da formatação do arg <init> */
    /* checando se é uma linha valida para receber  */
    if (ponteiro == NULL){
        return NULL;
    }

    /* tenho que verificar se é uma linha para o usuario requisitado , separar usuario,o hash e o script ,
     * colcar tudo na strutura do <SecondAuthData> e retornar um ponteiro pra ela */
    //<format><user><sep><hash><sep><script><end>
    
    
    // pegar o tamanho do usuario pra alocar só o que precisar e fazer o mesmo pro hash da senha e script
    char *username = NULL;
    username = calloc(sizeof(char),sizeof(char) * 50); 
    
    char *hash = NULL;
    hash = calloc(sizeof(char),sizeof(char) * 50); 
    
    char *script = NULL;
    script = calloc(sizeof(char),sizeof(char) * 50); 
   
    /* ponteiro sendo reduzido para logo depois do inicio da formatação*/
    ponteiro = strstr(ponteiro,init) + leninit;

    /* concatenando o usuario no array */
    strncat(username,ponteiro, strcspn(ponteiro,separator));
    
    /* ponteiro sendo reduzido para logo depois do primeiro separador */
    ponteiro = strstr(ponteiro,separator) + lenseparator;
    
    /* concatendando o hash da senha do usuario no array*/
    strncat(hash,ponteiro, strcspn(ponteiro,separator));

    /* ponteiro sendo reduzido para logo depois do segundo separador */
    ponteiro = strstr(ponteiro,separator) + lenseparator;
    
    /* concatenando o caminho para o script no array */
    strncat(script,ponteiro, strcspn(ponteiro,end));

#ifdef DEBUG

    printf("%s - %s - %s\n",username,hash,script);
#endif // DEBUG
    
    free(username);
    free(hash);
    free(script);


}

int lerarquivo(char *arquivo){
  /* opening the file in read mode */
    FILE *fp = fopen(arquivo,"r");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file\n");
        return 1;
    }

    char *line = NULL;   /* pointer to hold the address of the line buffer */
    size_t len = 0;      /* variable to store the size of the buffer */
    ssize_t read;        /* variable to store the number of characters read */

    /* read each line using getline until end of file is reached */
    while ((read = getline(&line, &len, fp)) != -1) {
        /* calling the parsing function */
        // tenho que implementar ainda :(
        parsing(line,"%$#",":",";");
    }

    /* Free the memory allocated by getline and close the file */
    free(line);
    fclose(fp);

  return EXIT_SUCCESS;
}
