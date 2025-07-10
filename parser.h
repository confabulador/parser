#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define INVALID_DATA NULL

typedef struct {
    char *user;         /* o nome do usuario na linha */
    char *pwhash;     /* os hash já calculado da(s) senha(s) de segurança para o usuario */
    char *script;     /* os scrips/programas dos repectivos hash de segurança */
}AuthData;

void limparstruct(AuthData *estrutura){
  free(estrutura -> user);
  free(estrutura -> pwhash);
  free(estrutura -> script);
  free(estrutura);
}


/*
 * essa função vai retornar o tamanho de uma string antes de uma segunda string ser detectada + 1
 * por exemplo: stringcount("ola mundo","mundo") vai retornar o numero 5
 * caso função n tenha encontrado nada ela vai retornar 0
 * recomendado verificar se a função retornou NULL caso não subtrair 1 para ter o valor real da string
 */
size_t stringcount(char *base, char *subject){
    
    char flag = 0;

    for (size_t counter = 0; base[counter] != '\0'; counter++){
        flag = 0;
        if (base[counter] == subject[0]){
            for (size_t inter_counter = 0; subject[inter_counter] != '\0'; inter_counter ++){
                if (base[counter+inter_counter] != subject[inter_counter]){
                    flag = 1;
                    break;
                }
           }
            if (flag == 0){
                // encontrou
                return counter + 1;
            }
        }   
    }
    // nada bateu
    return 0;
}

AuthData *parsing(char *linebuff,char *init,char *separator,char *end){
    size_t leninit = strlen(init);              /* tamanho da formatação inicial */
    size_t lenseparator = strlen(separator);    /* tamanho da formatação de separação */

    char *ponteiro = NULL;
    
    ponteiro = strstr(linebuff,init); /* string iniciando a partir da formatação do arg <init> */
    /* checando se é uma linha valida para receber  */
    if (ponteiro == NULL){
        return INVALID_DATA;
    }
    
    /* formatação tem que seguir esse padrão <format><user><sep><hash><sep><script><end> */
    
    char *username = NULL;
    char *hash = NULL;
    char *script = NULL;
   
    /* ponteiro sendo reduzido para logo depois do inicio da formatação*/
    ponteiro = strstr(ponteiro,init) + leninit;

    size_t lenuser = stringcount(ponteiro,separator);

    if(lenuser == 0){
#ifdef DEBUG
        printf("PRIMEIRO SEPARADOR - %p\n",ponteiro);
#endif // DEBUG
        return INVALID_DATA;
    } /* caso não tenha usuario vai retornar erro*/ 
    
    username = calloc(sizeof(char),sizeof(char) * (lenuser + 1) );
    /* verificando se foi alocado */ 
    if(username == NULL){
        return NULL;
    }
    
    lenuser -= 1;
    /* concatenando o usuario no array */
    strncat(username,ponteiro, lenuser);

    /* ponteiro sendo reduzido para logo depois do primeiro separador */
    ponteiro = strstr(ponteiro,separator);
    if(ponteiro == NULL){
        free(username);
        return INVALID_DATA;
    }

    ponteiro = ponteiro + lenseparator;
    size_t lenpass = stringcount(ponteiro,separator);   
    if(lenpass == 0){
#ifdef DEBUG
        printf("SEGUNDO SEPARADOR - %p\n",ponteiro);
#endif // DEBUG 
        free(username);
        return INVALID_DATA;
    } /* caso não tenha hash da senha vai retornar erro*/ 
    
    hash = calloc(sizeof(char),sizeof(char) * (lenpass + 1));
    /* verificando se foi alocado */
    if(hash == NULL){
        free(username);
        return NULL;
    }
    
    lenpass -= 1;
    /* concatendando o hash da senha do usuario no array*/
    strncat(hash,ponteiro, lenpass);

    /* ponteiro sendo reduzido para logo depois do segundo separador */
    ponteiro = strstr(ponteiro,separator);
    if(ponteiro == NULL){
        free(username);
        free(hash);
        return INVALID_DATA;
    }
    
    ponteiro = ponteiro + lenseparator;
    
    size_t lenscript = stringcount(ponteiro,end);
    if(lenscript == 0){
#ifdef DEBUG
    printf("FINAL - %p\n",ponteiro);
#endif // DEBUG        
        free(username);
        free(hash);
        return INVALID_DATA;
    } /* caso não tenha script vai retornar erro*/ 
    
    script = calloc(sizeof(char),sizeof(char) * (lenscript + 1)); 
    /* verificando se foi alocado */
    if(script == NULL){
        free(username);
        free(hash);
        return NULL;
    }
    
    lenscript -=1;
    /* concatenando o caminho para o script no array */
    strncat(script,ponteiro, lenscript);
   
    /* ponteiro sendo reduzido para o final */
    ponteiro = strstr(ponteiro,end);
    if(ponteiro == NULL){
        free(username);
        free(hash);
        free(script);
        return INVALID_DATA;
    }

#ifdef DEBUG

    printf("%s - %s - %s\n",username,hash,script);
#endif // DEBUG
    
    /* se tudo deu certo os dados vão ser passados para o ponteiro da estrutura AuthData
     * e logo em seguida vai ser retornado */
    
    AuthData *SecondAuthData = malloc(sizeof(AuthData));
    /* verificando se foi alocado */
    if(SecondAuthData == NULL){
        free(username);
        free(hash);
        free(script);
        return NULL;
    }

    SecondAuthData -> user = username;
    SecondAuthData -> pwhash = hash;
    SecondAuthData -> script = script;
    
    return SecondAuthData;
}

AuthData *lerarquivo(char *arquivo, char *init, char *separator, char *end, char *usuario){
  /* opening the file in read mode */
    FILE *fp = fopen(arquivo,"r");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file\n");
        return INVALID_DATA;
    }

    char *line = NULL;   /* pointer to hold the address of the line buffer */
    size_t len = 0;      /* variable to store the size of the buffer */
    ssize_t read;        /* variable to store the number of characters read */

    /* read each line using getline until end of file is reached */
    while ((read = getline(&line, &len, fp)) != -1) {
        /* calling the parsing function */
        //parsing(line, init, separator, end);
        AuthData *estrutura = parsing(line, init, separator, end);
        if(estrutura != INVALID_DATA){
            if(strcmp(usuario, estrutura -> user) == 0){
                free(line);
                fclose(fp);
                return estrutura;
            }
            limparstruct(estrutura);
        }
    }

    /* Free the memory allocated by getline and close the file */
    free(line);
    fclose(fp);
    
    return INVALID_DATA;
}
