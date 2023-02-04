#include "palavra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct palavra
{
    char *palavra;
    Tabela *tabela;
    int n_documentos;
    int tam_tabela;
};

Palavra palavra_criar(char *palavra){
    Palavra p = (Palavra)calloc(1, sizeof(struct palavra));
    p->palavra = strdup(palavra);
    p->n_documentos = 0;
    p->tam_tabela = 100;
    p->tabela = (Tabela *)calloc(p->tam_tabela, sizeof(Tabela));
    return p;
}

int compara_palavra_string(const void *palavra, const void *p1){
    Palavra pr1 = *(Palavra *)p1;
    return strcmp((char *)palavra, pr1->palavra);
}

void palavra_add_tabela(Palavra p, int idx_doc, int verify){
    int achou = -1;
    if(verify){
        achou = tabelas_possui_idx(p->tabela, idx_doc, p->n_documentos);
    }
    if(achou == -1){
        if(p->n_documentos >= p->tam_tabela){
            p->tam_tabela += 200;
            p->tabela = (Tabela *)realloc(p->tabela, p->tam_tabela * sizeof(Tabela));
        }
        p->tabela[p->n_documentos] = tabela_criar(idx_doc);
        p->n_documentos += 1;
    }
    else
    {
        tabela_add_freq(palavra_get_tab(p, achou));
    }
    
}

void palavra_calc_tfidf(Palavra p, float idf){
    for(int i = 0; i < p->n_documentos; i++){
        tabela_calc_tfidf(p->tabela[i], idf);
    }
}

Palavra palavra_organizar_tabela(Palavra p, int compare(const void*, const void*)){
    qsort(p->tabela, p->n_documentos, sizeof(Tabela), compare);
}

int palavra_get_n_docs(Palavra p){
    return p->n_documentos;
}

int compara_palavra_tfidf(const void *p1, const void *p2);

int compara_palavras(const void *p1, const void *p2){
    Palavra pr1 = *(Palavra *)p1;
    Palavra pr2 = *(Palavra *)p2;
    return strcmp(pr1->palavra, pr2->palavra);
}

Tabela palavra_get_tab(Palavra p, int idx){
    if(idx < 0 || idx >= palavra_get_n_docs(p)){
        return NULL;
    }
    return p->tabela[idx];
}

void palavra_salvar(Palavra p, FILE * fpalavra){
    int n = strlen(p->palavra) + 1;
    fwrite(&n, 1, sizeof(int), fpalavra);
    fwrite(p->palavra, n, sizeof(char), fpalavra);
    fwrite(&p->n_documentos, 1, sizeof(int), fpalavra);
    //qsort(p->tabela, p->n_documentos, sizeof(Tabela), compara_idx);
    for(int i = 0; i < p->n_documentos; i++){
        tabela_salvar(p->tabela[i], fpalavra);
    }
}

Palavra palavra_carregar(FILE * fpalavra){
    int n = 0;
    int n_docs = 0;
    fread(&n, 1, sizeof(int), fpalavra);
    char *string = (char *)calloc(n, sizeof(char));
    Palavra p = palavra_criar(string);
    fread(p->palavra, n, sizeof(char), fpalavra);
    fread(&n_docs, 1, sizeof(int), fpalavra);
    for(int i = 0; i < n_docs; i++){
        fread(&n, 1, sizeof(int), fpalavra);
        palavra_add_tabela(p, n, 0);
        tabela_carregar(p->tabela[i], fpalavra);
    }
    free(string);
    return p;
}

void imprimir_palavra(Palavra p){
    printf("%s\n", p->palavra);
    printf("N docs: %d\n", p->n_documentos);
    for(int i = 0; i < p->n_documentos; i++){
        tabela_imprimir(p->tabela[i]);
    }
}

void palavra_destruir(Palavra p){
    free(p->palavra);
    for(int i = 0; i < p->n_documentos; i++){
        tabela_destruir(p->tabela[i]);
    }
    free(p->tabela);
    free(p);
}

char * palavra_get_string(Palavra p){
    return p->palavra;
}

Palavra palavra_copiar(Palavra p){
    Palavra p1 = (Palavra)calloc(1, sizeof(struct palavra));
    p1->tam_tabela = p->tam_tabela;
    p1->n_documentos = p->n_documentos;
    p1->palavra = strdup(p->palavra);
    p1->tabela = (Tabela *)calloc(p->tam_tabela, sizeof(Tabela));
    for(int i = 0; i < p1->n_documentos; i++){
        p1->tabela[i] = copiar_tabela(p->tabela[i]);
    }
    return p1;
}

