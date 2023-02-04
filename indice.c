#include <stdio.h>
#include <documento.h>
#include <bancodedados.h>
#include <tabela.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char*argv[]){

    if(argc < 3){
        printf("Falta argumentos.\n");
        exit(1);
    }

    FILE * documentos;
    documentos = fopen(argv[1], "r");

    if(!documentos){
        printf("Erro ao abrir o documento\n");
        exit(1);
    }

    char *linha = NULL;
    size_t bufsize = 0;

    char *caminho = NULL;
    char *classe = NULL;
    Documento doc = NULL;
    BancoDeDados bd = bd_criar(argv[1]);
    getline(&linha, &bufsize, documentos);
    while(!feof(documentos)){
        caminho = strtok(linha, " ");
        classe = strtok(NULL, "\n");
        doc = documento_cria(caminho, classe);
        bd_adicionar_doc(bd, doc);
        le_palavras(doc, bd);
        getline(&linha, &bufsize, documentos);
    }
    free(linha);
    bd_organizar_palavras(bd);
    calcula_tfidf(bd);
    bd_salvar(bd, argv[2]);
    bd_destruir(bd);
    fclose(documentos);

    return 0;
}
